#include <SPI.h>
#include <SD.h>

#define BUF_LEN 1024
#define GCODE_BUF_LEN 100

File myFile;

char gcodeBufFull = 0;
char buffer[1024];
int bufLength = BUF_LEN;
char gcodeBuf[GCODE_BUF_LEN][50];
int bufIndex = 0;
int gcodeIndex = 0;
unsigned int bufTotalLen = 0;
int numOfGcode = 0;
int bufLen = 0;
int gcodeBufHead = 0;
int gcodeBufTail = 0;

enum {
	NOT_FULL = 0,
	FULL
};

enum {
	NOT_EMPTY,
	EMPTY
};

char bufFullOrNot() {
	if (gcodeBufHead > gcodeBufTail) {
		int tmpBufLen = gcodeBufHead - gcodeBufTail;

		if (tmpBufLen > 0) {
			return NOT_FULL;
		} else {
			return FULL;
		}
	} else if (gcodeBufHead < gcodeBufTail) {

		int tmpBufLen = gcodeBufHead - gcodeBufTail + GCODE_BUF_LEN;
		if (tmpBufLen > 0) {
			return NOT_FULL;
		} else {
			return FULL;
		}
	} else if (gcodeBufHead == gcodeBufTail) {
		return NOT_FULL;
	}
}

char bufIsEmpty() {
	if (gcodeBufHead == gcodeBufTail) {
		return EMPTY;
	} else {
		return NOT_EMPTY;
	}
}

char bufIsFull() {
	int tmpBufLen = gcodeBufHead - gcodeBufTail + GCODE_BUF_LEN;
	if (gcodeBufHead == gcodeBufTail) {
		return EMPTY;
	} else {
		return NOT_EMPTY;
	}
}

void setup() {
  Serial.begin(115200);

  Serial.print("Initializing SD card...");
  if (!SD.begin(31)) {
    Serial.println("initialization failed!");
    while(1);
  }
  Serial.println("initialization done.");
  
  myFile = SD.open("gcode.gco");
	if (!myFile) {
		Serial.println("error opening test.txt");

	} 
}

void loop() {

	if (myFile) {

		if (myFile.available()) {

			gcodeBufFull = bufFullOrNot();

			if (gcodeBufFull == NOT_FULL) {

				char tmpRead = myFile.read();

				gcodeBuf[gcodeBufHead][bufIndex++] = tmpRead;

				if (tmpRead == '\n') {

					String printBuf = "[" + String(gcodeBufHead) + "]" + gcodeBuf[gcodeBufHead];

					Serial.print(printBuf);
					bufIndex = 0;
					gcodeBufHead++;

					if (gcodeBufHead == GCODE_BUF_LEN) {
						gcodeBufHead = 0;
					}

					if (gcodeBufHead == gcodeBufTail) {

						gcodeBufFull = 1;
						gcodeBufHead--;

						if (gcodeBufHead < 0) {
							gcodeBufHead= GCODE_BUF_LEN - 1;
						}
					}

					delay(100);

				}
			}
		} else {
			myFile.close();
		}
	} 
}