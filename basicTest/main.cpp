#include "sane/sane.h"
#include <iostream>
using namespace std;

void printStatus(SANE_Status status) {
    cout << "status: " << sane_strstatus(status) << endl;
}

void printDevice(const SANE_Device* device) {
    cout << "device:" << endl <<
        "\tname = " << device->name << endl <<
        "\tvendor = " << device->vendor << endl <<
        "\tmodel = " << device->model << endl <<
        "\ttype = " << device->type << endl;
}

string typeToString(const SANE_Value_Type& type) {
    switch(type) {
        case SANE_TYPE_BOOL: return "bool";
        case SANE_TYPE_INT: return "int";
        case SANE_TYPE_FIXED: return "fixed";
        case SANE_TYPE_STRING: return "string";
        case SANE_TYPE_BUTTON: return "button";
        case SANE_TYPE_GROUP: return "group";
        default: return "?";
    }
}

string unitToString(const SANE_Unit& unit) {
    switch(unit) {
        case SANE_UNIT_BIT: return "bits";
        case SANE_UNIT_DPI: return "dpi";
        case SANE_UNIT_MICROSECOND: return "microseconds";
        case SANE_UNIT_MM: return "millimeters";
        case SANE_UNIT_NONE: return "unitless";
        case SANE_UNIT_PERCENT: return "percent";
        case SANE_UNIT_PIXEL: return "pixel";
        default: return "?";
    }
}

void printDescriptor(const SANE_Option_Descriptor* descriptor) {
    cout << descriptor->title << " " <<
        "(" << typeToString(descriptor->type) <<
        "/" << unitToString(descriptor->unit) << ")" <<
        "[" << descriptor->size << "] ";
    switch(descriptor->constraint_type) {
        case SANE_CONSTRAINT_NONE:
            cout << "any value";
            break;
        case SANE_CONSTRAINT_STRING_LIST:
            cout << "{string list...}";
            break;
        case SANE_CONSTRAINT_WORD_LIST:
            cout << "{word list...}";
            break;
        case SANE_CONSTRAINT_RANGE:
            const SANE_Range* range = descriptor->constraint.range;
            cout << range->min << " to " << range->max;
            if(range->quant != 0)
                cout << " in " << range->quant;
            break;
    }
    cout << endl;
}

int main() {
    SANE_Int versionCode;
    cout << "sane_init" << endl;
    printStatus(sane_init(&versionCode, NULL));
    cout << "running SANE v" << SANE_VERSION_MAJOR(versionCode) << "." << SANE_VERSION_MINOR(versionCode) << endl;

    const SANE_Device** deviceList;
    cout << "sane_get_devices" << endl;
    printStatus(sane_get_devices(&deviceList, true));

    int curDevice;
    for(curDevice = 0; deviceList[curDevice] != NULL; curDevice++)
        printDevice(deviceList[curDevice]);
    int totalDevices = curDevice;
    cout << totalDevices << " devices total" << endl;

    if(totalDevices > 0) {
        SANE_String_Const name = deviceList[0]->name;
        SANE_Handle handle;
        cout << "sane_open(" << name << ")" << endl;
        printStatus(sane_open(name, &handle));

        // use sane_get_option_descriptor to get options
        int totalDescriptors;
        sane_control_option (handle, 0, SANE_ACTION_GET_VALUE, &totalDescriptors, 0);
        cout << totalDescriptors << " descriptors total:" << endl;
        for(int i = 0; i < totalDescriptors; i++) {
            const SANE_Option_Descriptor* cur = sane_get_option_descriptor(handle, i);
            cout << i << ": ";
            printDescriptor(cur);
        }

        cout << "sane_start" << endl;
        printStatus(sane_start(handle));

        SANE_Parameters parameters;
        printStatus(sane_get_parameters(handle, &parameters));
        SANE_Int maxSize = parameters.bytes_per_line;
        SANE_Byte* buffer = new SANE_Byte[maxSize];
        SANE_Int size;

        SANE_Status status = SANE_STATUS_GOOD;
        cout << "while(status == SANE_STATUS_GOOD)..." << endl;
        while(status == SANE_STATUS_GOOD) {
            status = sane_read(handle, buffer, maxSize, &size);
            for(int i = 0; i < 80; i++) {
                if((int) buffer[i] > 128)
                    cout << " ";
                else
                    cout << "X";
            }
            cout << endl;
        }

        cout << "final status is: ";
        printStatus(status);

        cout << "sane_cancel" << endl;
        sane_cancel(handle);
        cout << "sane_close" << endl;
        sane_close(handle);

        delete [] buffer;
    }

    sane_exit();
    return 0;
}
