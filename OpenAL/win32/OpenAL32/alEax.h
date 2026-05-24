#ifndef _AL_EAX_H_
#define _AL_EAX_H_

#include "al\altypes.h"

#ifdef __cplusplus
extern "C" {
#endif

ALboolean CheckEAXSupport();
ALenum    EAXGet(const struct _GUID* propertySetID, ALuint property, ALuint source, ALvoid* value, ALuint size);
ALenum    EAXSet(const struct _GUID* propertySetID, ALuint property, ALuint source, ALvoid* value, ALuint size);

#ifdef __cplusplus
}
#endif

#endif