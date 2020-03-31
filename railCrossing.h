enum railCrossingStates {
	railCrossingIDLE,
	closeCrossing,
	openCrossing,
	waitCrossing };

extern bool railCrossing(void); 
extern void railCrossingInit();
extern void railCrossingSetState(unsigned char);
extern unsigned char railCrossingGetState(void);
