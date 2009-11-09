#ifndef __EVENT_H__
#define __EVENT_H__

#ifndef _WIN32 


#ifdef __cplusplus
extern "C" {
#endif


HANDLE EventCreate();
void WaitEvent(HANDLE hEvent);
void SendEvent(HANDLE hEvent); 
void EventClose(HANDLE hEvent);

HANDLE SemaCreate(x,y) 
#define SemaWait(x)      
#define SemaRelease(x,y)   
#define SemaClose(x)       


#ifdef __cplusplus
}
#endif

#endif /* _WIN32 */
#endif /* __EVENT_H__ */