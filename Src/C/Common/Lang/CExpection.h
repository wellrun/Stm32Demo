#ifndef C_EXPECTION_H_
#define C_EXPECTION_H_

#include "Lang.h"
#include "setjmp.h"
#ifdef __cplusplus
extern "C" {
#endif
	
#define MSG_THROW_VAL 255

	
#define TRY	expId = setjmp(expectionBuf) ; \
				if(expId == 0)

#define CATCH(exp) else if(expId == exp)

#define FINALLY else

#define THROW(e) longjmp(expectionBuf, e)

#ifndef RELEASE
	#define THROW_MSG(msg) do{                         \
		cExpectMsg = msg;                              \
		longjmp(expectionBuf, MSG_THROW_VAL);          \
	} while(0)
#else 
	#define THROW_MSG(msg) while(1)
#endif //RELEASE
#define CATCH_MSG if(expId == MSG_THROW_VAL)

#define EXPECTION_MSG cExpectMsg

extern jmp_buf expectionBuf;
extern int expId;
extern char *cExpectMsg;


#ifdef __cplusplus
}
#endif

#endif
