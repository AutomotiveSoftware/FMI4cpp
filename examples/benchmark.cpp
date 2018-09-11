//
// Created by LarsIvar on 10.09.2018.
//

#include <iostream>
#include <fmicpp/fmi2/fmicpp.hpp>
#include <fmicpp/tools/os_util.hpp>

using namespace std;
using namespace fmicpp::fmi2::import;

const double stop = 10.0;
const double step_size = 1E-4;

int main() {

    const string fmu_path = string(getenv("TEST_FMUs"))
                            + "/FMI_2.0/CoSimulation/" + getOs() +
                            "/20sim/4.6.4.8004/ControlledTemperature/ControlledTemperature.fmu";

    Fmu fmu(fmu_path);
    const auto slave = fmu.asCoSimulationFmu().newInstance();
    slave->init();

    clock_t begin = clock();

    vector<fmi2Real > ref(2);
    vector<fmi2ValueReference > vr = {slave->getValueReference("Temperature_Reference"),
                                         slave->getValueReference("Temperature_Room")};


    double t;
    while ( (t = slave->getSimulationTime() ) <= (stop-step_size) ) {
        fmi2Status status = slave->doStep(step_size);
        if (status != fmi2OK) {
            cout << "Error! step returned with status: " << to_string(status) << endl;
            break;
        }
        slave->readReal(vr, ref);
//        cout << "t=" << t << ", Temperature_Reference=" << ref[0] << ", Temperature_Room=" << ref[1] << endl;
    }

    clock_t end = clock();

    double elapsed_secs = double(end-begin) / CLOCKS_PER_SEC;
    cout << "elapsed=" << elapsed_secs << "s" << endl;

    slave->terminate();

}