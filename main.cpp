#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <cstring>
#include <fstream>

using namespace std;

int main() {
    int usb = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);

    if (usb < 0) {
        cerr << "Unable to open /dev/ttyUSB0" << endl;

        return 1;
    }

    termios tty;
    memset(&tty, 0, sizeof(tty));

    if (tcgetattr(usb, &tty) != 0) {
        cerr << "Error setting termios attributes" << endl;

        return 1;
    }

    tty.c_iflag = IGNCR;
    tty.c_cflag = CS8;
    tty.c_lflag = ICANON;


    cfsetospeed(&tty, B921600);
    cfsetispeed(&tty, B921600);

    tcsetattr(usb, TCSANOW, &tty);

    ofstream file;
    file.open("../log", ios_base::out);

    char buffer[100] = { '\0' };

    int bytes_read;

    // read first incomplete line
    while (read(usb, buffer, 100) <= 0);

    while (true) {
        if ( (bytes_read = read(usb, buffer, 100)) > 0) {
            buffer[bytes_read] = '\0';
            file << buffer;
        }
    }

    return 0;
}