/*
 * THIS FILE IS AUTOMATICALLY GENERATED
 *
 * I2C-Generator: 0.2.0
 * Yaml Version: 0.1.0
 * Template Version: local build
 */
/*
 * Copyright (c) 2021, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "SensirionI2CSgp40.h"
#include "Arduino.h"
#include "SensirionCore.h"
#include <Wire.h>

#define SGP40_I2C_ADDRESS 0x59

SensirionI2CSgp40::SensirionI2CSgp40() {
}

void SensirionI2CSgp40::begin(TwoWire& i2cBus) {
    _i2cBus = &i2cBus;
}

uint16_t SensirionI2CSgp40::measureRaw(uint16_t relativeHumidity,
                                       uint16_t temperature,
                                       uint16_t& srawVoc) {
    uint16_t error;
    uint8_t buffer[8];
    SensirionI2CTxFrame txFrame(buffer, 8);

    error = txFrame.addCommand(0x260F);
    error |= txFrame.addUInt16(relativeHumidity);
    error |= txFrame.addUInt16(temperature);
    if (error) {
        return error;
    }

    error = SensirionI2CCommunication::sendFrame(SGP40_I2C_ADDRESS, txFrame,
                                                 *_i2cBus);
    if (error) {
        return error;
    }

    delay(30);

    SensirionI2CRxFrame rxFrame(buffer, 8);
    error = SensirionI2CCommunication::receiveFrame(SGP40_I2C_ADDRESS, 3,
                                                    rxFrame, *_i2cBus);
    if (error) {
        return error;
    }

    error |= rxFrame.getUInt16(srawVoc);
    return error;
}

uint16_t SensirionI2CSgp40::measureTest(uint16_t& testResult) {
    uint16_t error;
    uint8_t buffer[3];
    SensirionI2CTxFrame txFrame(buffer, 3);

    error = txFrame.addCommand(0x280E);
    if (error) {
        return error;
    }

    error = SensirionI2CCommunication::sendFrame(SGP40_I2C_ADDRESS, txFrame,
                                                 *_i2cBus);
    if (error) {
        return error;
    }

    delay(320);

    SensirionI2CRxFrame rxFrame(buffer, 3);
    error = SensirionI2CCommunication::receiveFrame(SGP40_I2C_ADDRESS, 3,
                                                    rxFrame, *_i2cBus);
    if (error) {
        return error;
    }

    error |= rxFrame.getUInt16(testResult);
    return error;
}

uint16_t SensirionI2CSgp40::turnHeaterOff() {
    uint16_t error;
    uint8_t buffer[2];
    SensirionI2CTxFrame txFrame(buffer, 2);

    error = txFrame.addCommand(0x3615);
    if (error) {
        return error;
    }

    error = SensirionI2CCommunication::sendFrame(SGP40_I2C_ADDRESS, txFrame,
                                                 *_i2cBus);
    delay(1);
    return error;
}
