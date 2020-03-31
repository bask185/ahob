enum readTrack1States {
	readTrack1IDLE,
	monitorSw1Sw2,
	waitSw1,
	waitSw2,
	waitTrain2passSw1,
	waitTrain2passSw2 };

extern bool readTrack1(void); 
extern void readTrack1Init();
extern void readTrack1SetState(unsigned char);
extern unsigned char readTrack1GetState(void);

extern unsigned char track1occupied;