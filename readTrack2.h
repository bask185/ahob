enum readTrack2States {
	readTrack2IDLE,
	monitorSw3Sw4,
	waitSw3,
	waitSw4,
	waitTrain2passSw3,
	waitTrain2passSw4 };

extern bool readTrack2(void); 
extern void readTrack2Init();
extern void readTrack2SetState(unsigned char);
extern unsigned char readTrack2GetState(void);

extern unsigned char track2occupied;