

#ifndef EOS_ASSERT_H
#define EOS_ASSERT_H

#ifndef _DEBUG
    inline void EosAssert(bool, const char*) { (void)0; }/* Remove asserted code during Release mode */
#else
#include "../Src/Engine/Frameworks/Logger/Logger.h"
#include "../Src/pch.h"
#define EosAssert(condition, loggedMessage) /* Logs a Error message into a file if condition is FALSE */ \
        { \
			if (!(condition))\
			{\
				std::stringstream EosAssertStringStream; \
				EosAssertStringStream << "ASSERT FAILED: " << #loggedMessage << " @ " << __FILE__ << " (" << __LINE__ << ")\n"; \
				PE_CORE_ERROR(EosAssertStringStream.str()); \
				assert(condition); \
			}\
        }


#endif

#endif
