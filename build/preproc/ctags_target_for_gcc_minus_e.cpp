# 1 "c:\\Users\\USER\\Documents\\PlatformIO\\Projects\\Narsha\\src\\main\\test_sdcard_seek_read\\test_sdcard_seek_read.ino"
# 2 "c:\\Users\\USER\\Documents\\PlatformIO\\Projects\\Narsha\\src\\main\\test_sdcard_seek_read\\test_sdcard_seek_read.ino" 2
# 3 "c:\\Users\\USER\\Documents\\PlatformIO\\Projects\\Narsha\\src\\main\\test_sdcard_seek_read\\test_sdcard_seek_read.ino" 2

File myFile;

char buffer[100];

void setup() {
  Serial.begin(115200);

  Serial.print("Initializing SD card...");
  if (!SD.begin(31)) {
    Serial.println("initialization failed!");
    while(1);
  }
  Serial.println("initialization done.");

  myFile = SD.open("gcode.gco");
  if (myFile) {
    // myFile.seek(0x40);
    myFile.read(buffer, 50);
    // for(int i=0;i<16;i++) Serial.println(buffer[i], char);
    for(int i=0;i<16;i++) Serial.println((char)buffer[i]);

    myFile.close();
  } else {
    Serial.println("error opening test.txt");
  }
}

void loop() {


}
