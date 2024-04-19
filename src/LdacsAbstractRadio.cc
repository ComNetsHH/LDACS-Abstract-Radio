// The LDACS Abstract Radio models the radio communications for accurate LDACS air-to-air simulation.
// Copyright (C) 2024  Musab Ahmed, Konrad Fuger, Koojana Kuladinithi, Andreas Timm-Giel, Institute of Communication Networks, Hamburg University of Technology, Hamburg, Germany
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "LdacsAbstractRadio.h"
#include "inet/physicallayer/common/packetlevel/RadioMedium.h"
#include "inet/physicallayer/unitdisk/UnitDiskReception.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/INETMath.h"

Register_Class(LdacsAbstractRadio);

LdacsAbstractRadio::LdacsAbstractRadio() : 
    UnitDiskRadio()
{
}

LdacsAbstractRadio::~LdacsAbstractRadio() {
    // NOTE: can't use the medium module here, because it may have been already deleted
    cModule *medium = getSimulation()->getModule(mediumModuleId);
    if (medium != nullptr)
        check_and_cast<IRadioMedium *>(medium)->removeRadio(this);
    cancelAndDelete(transmissionTimer);
    cancelAndDelete(switchTimer);
    for (auto timer : allReceptionTimers)
        cancelAndDelete(timer);
    allReceptionTimers.clear();
}

void LdacsAbstractRadio::startReception(cMessage *timer, IRadioSignal::SignalPart part)
{
    auto signal = static_cast<Signal *>(timer->getControlInfo());
    auto arrival = signal->getArrival();
    auto reception = signal->getReception();
// TODO: should be this, but it breaks fingerprints: if (receptionTimer == nullptr && isReceiverMode(radioMode) && arrival->getStartTime(part) == simTime()) {
    if (isReceiverMode(radioMode) && arrival->getStartTime(part) == simTime()) {
        auto transmission = signal->getTransmission();
        // auto isReceptionAttempted = medium->isReceptionAttempted(this, transmission, part);
        // All receivable signals will be attempted
        auto power = check_and_cast<const UnitDiskReception *>(reception)->getPower();
        auto isReceptionAttempted = (power == UnitDiskReception::POWER_RECEIVABLE);
        EV_INFO << "Reception started: " << (isReceptionAttempted ? "\x1b[1mattempting\x1b[0m" : "\x1b[1mnot attempting\x1b[0m") << " " << (ISignal *)signal << " " << IRadioSignal::getSignalPartName(part) << " as " << reception << endl;
        if (isReceptionAttempted) {
            receptionTimer = timer;
            // Add the timer to the list of all reception timers that are attempted
            allReceptionTimers.push_back(timer);
            emit(receptionStartedSignal, check_and_cast<const cObject *>(reception));
        }
    }
    else
        EV_INFO << "Reception started: \x1b[1mignoring\x1b[0m " << (ISignal *)signal << " " << IRadioSignal::getSignalPartName(part) << " as " << reception << endl;
    timer->setKind(part);
    scheduleAt(arrival->getEndTime(part), timer);
    updateTransceiverState();
    updateTransceiverPart();
    // TODO: move to radio medium
    check_and_cast<RadioMedium *>(medium)->emit(IRadioMedium::signalArrivalStartedSignal, check_and_cast<const cObject *>(reception));
}

void LdacsAbstractRadio::endReception(cMessage *timer)
{
    auto part = (IRadioSignal::SignalPart)timer->getKind();
    auto signal = static_cast<Signal *>(timer->getControlInfo());
    auto arrival = signal->getArrival();
    auto reception = signal->getReception();
    auto it = std::find(allReceptionTimers.begin(), allReceptionTimers.end(), timer);
    bool isReceptionAttempted = (it != allReceptionTimers.end());
    bool isReceptionSuccessful = isReceptionAttempted;
    if (isReceptionAttempted && isReceiverMode(radioMode) && arrival->getEndTime() == simTime()) {
        auto transmission = signal->getTransmission();
// TODO: this would draw twice from the random number generator in isReceptionSuccessful: auto isReceptionSuccessful = medium->isReceptionSuccessful(this, transmission, part);
        // auto isReceptionSuccessful = medium->getReceptionDecision(this, signal->getListening(), transmission, part)->isReceptionSuccessful();
        EV_INFO << "Reception ended: " << (isReceptionSuccessful ? "\x1b[1msuccessfully\x1b[0m" : "\x1b[1munsuccessfully\x1b[0m") << " for " << (ISignal *)signal << " " << IRadioSignal::getSignalPartName(part) << " as " << reception << endl;
        auto macFrame = medium->receivePacket(this, signal);
        // TODO: FIXME: see handling packets with incorrect PHY headers in the TODO file
        decapsulate(macFrame);
        sendUp(macFrame);
        emit(receptionEndedSignal, check_and_cast<const cObject *>(reception));
    }
    else
        EV_INFO << "Reception ended: \x1b[1mignoring\x1b[0m " << (ISignal *)signal << " " << IRadioSignal::getSignalPartName(part) << " as " << reception << endl;
    updateTransceiverState();
    updateTransceiverPart();
    if(timer == receptionTimer)
        receptionTimer = nullptr;
    if (it != allReceptionTimers.end()) {
    allReceptionTimers.erase(it); // Always remove the timer upon ending reception
    }
    delete timer;
    // TODO: move to radio medium
    check_and_cast<RadioMedium *>(medium)->emit(IRadioMedium::signalArrivalEndedSignal, check_and_cast<const cObject *>(reception));
}
