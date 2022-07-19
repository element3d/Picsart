
#ifndef __SplashBase_H__
#define __SplashBase_H__

#include <e3/ViewFactory.h>


class SplashBase : public e3::Element
{
public:
    struct __Id
    {
        
    };

    struct Id
    {
        
    };

    SplashBase(e3::Element* pParent = nullptr);

 
protected:
	e3::Element* mShape = nullptr;
e3::Element* mShape2 = nullptr;
e3::Element* mLogo = nullptr;
e3::Element* mTextCont = nullptr;


};

#endif // __SplashBase_H__

