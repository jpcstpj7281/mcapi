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



#ifdef __cplusplus
}
#endif

#endif /* _WIN32 */
#endif /* __EVENT_H__ */