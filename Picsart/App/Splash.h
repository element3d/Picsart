
#ifndef Splash_H_
#define Splash_H_

#include "private/SplashBase.h"

class E3_EXPORT Splash : public SplashBase
{
public:
	Splash(e3::Element* pParent = nullptr);

    void Animate(std::string text, int i);
};

#endif // Splash_H_

