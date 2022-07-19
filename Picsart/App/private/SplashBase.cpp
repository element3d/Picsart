#include "SplashBase.h"

#include <e3/ViewFactory.h>
#include "PicsartValues.h"

SplashBase::SplashBase(e3::Element* pParent)
	: e3::Element(pParent)
{
        this->SetBackgroundColor(glm::vec4(1.000000, 253.000000, 255.000000, 255.000000));
        this->SetBackgroundImageFit((e3::EBackgroundSize)1);
        this->SetOrientation((e3::EOrientation)1);
    mShape = e3::ViewFactory::CreateShape( e3::EOrientation::Horizontal);
    AddElement(mShape);
        mShape->SetHeight("80%");
        mShape->SetAspectRatio(1.000000);
        mShape->SetPositionType((e3::EPositionType)1);
        mShape->SetShapeType((e3::EShapeType)1);
        mShape->SetBackgroundColor(glm::vec4(255.000000, 10.000000, 239.000000, 255.000000));
    mShape2 = e3::ViewFactory::CreateShape( e3::EOrientation::Horizontal);
    AddElement(mShape2);
        mShape2->SetHeight("50%");
        mShape2->SetAspectRatio(1.000000);
        mShape2->SetPositionType((e3::EPositionType)1);
        mShape2->SetShapeType((e3::EShapeType)1);
        mShape2->SetBackgroundColor(glm::vec4(255));
    mLogo = e3::ViewFactory::CreateShape( e3::EOrientation::Horizontal);
    AddElement(mLogo);
        mLogo->SetWidth("250dp");
        mLogo->SetAspectRatio(1.000000);
        mLogo->SetBackgroundImage("Picsart/logo1.png");
    mTextCont = e3::ViewFactory::CreateShape( e3::EOrientation::Horizontal);
    AddElement(mTextCont);

}
