/*
 * The MIT License
 *
 * Copyright 2017-2018 Norwegian University of Technology
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING  FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <iostream>
#include <fmicpp/fmi2/fmicpp.hpp>
#include <fmicpp/tools/os_util.hpp>

using namespace std;
using namespace fmicpp::fmi2;

const fmi2ValueReference vr = 2;
const double stop = 12.0;
const double step_size = 1E-5;

int main() {

    const string fmu_path = string(getenv("TEST_FMUs"))
                            + "/FMI_2.0/CoSimulation/" + getOs() +
                            "/20sim/4.6.4.8004/TorsionBar/TorsionBar.fmu";

    import::Fmu fmu(fmu_path);
    const auto slave = fmu.asCoSimulationFmu().newInstance();
    slave->init();

    clock_t begin = clock();

    double t;
    double ref;
    while ((t = slave->getSimulationTime()) <= (stop - step_size)) {
        fmi2Status status = slave->doStep(step_size);
        if (status != fmi2OK) {
            cout << "Error! step returned with status: " << to_string(status) << endl;
            break;
        }
        slave->readReal(vr, ref);
    }

    clock_t end = clock();

    long elapsed_ms =  (long) ((double(end-begin) / CLOCKS_PER_SEC) * 1000.0);
    cout << "elapsed=" << elapsed_ms << "ms" << endl;

    slave->terminate();

}