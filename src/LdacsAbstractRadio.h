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

#ifndef LDACSABSTRACTRADIO_H_
#define LDACSABSTRACTRADIO_H_

#include "inet/physicallayer/unitdisk/UnitDiskRadio.h"

using namespace inet;
using namespace inet::physicallayer;

class LdacsAbstractRadio: public UnitDiskRadio {
    protected:
        /** All reception timers including timers for attempted receptions. */
        std::vector<cMessage *> allReceptionTimers;

        virtual void startReception(cMessage *timer, IRadioSignal::SignalPart part) override;
        virtual void endReception(cMessage *timer) override;
    public:
        LdacsAbstractRadio();
        virtual ~LdacsAbstractRadio();
};

#endif /* LDACSABSTRACTRADIO_H_ */
