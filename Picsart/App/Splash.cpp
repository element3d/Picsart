#include "Splash.h"
#include <e3/Application.h>
#include "Main.h"
class PText : public e3::Text
{
public:
    PText() : e3::Text() {}

    e3::Text* next = nullptr;
};

Splash::Splash(e3::Element* pParent)
	: SplashBase(pParent)
{

    e3::OnTimeoutCallback* c = new e3::OnTimeoutCallback([this](){
        GetApplication()->PushElement(new Main());
    });
  e3::Timeout* pT = new e3::Timeout();
  pT->Start(1.5, c);

    std::string text = "picsart";
    int i = 0;
    Animate(text, i);

    {
        e3::Animation* a = new e3::Animation();
        a->Start(0.6, e3::EAnimation::EaseOutQuad, [this](float v){
            mTextCont->SetTranslation(glm::vec3(e3::Dim("60dp") + v * e3::Dim("5dp"), 0, 0));
        }, [](){

        });
    }

    {
        mShape2->SetOpacity(0);
        mLogo->SetOpacity(0);
        e3::Animation* a = new e3::Animation();
        a->Start(0.3, [this](float v){
            mShape->SetScale(glm::vec3(v), e3::ETransformAlignment::Center);
        }, [this](){

            {
//                e3::Animation* a1 = new e3::Animation();
//                a1->Start(0.2, e3::EAnimation::EaseOutQuad, [this](float v){
////                    mShape2->SetOpacity(v);

//                }, [](){

//                });

                e3::Animation* a = new e3::Animation();
                a->Start(1.0, e3::EAnimation::EaseOutBack, [this](float v){
                    mShape2->SetOpacity(1);
                    mShape2->SetScale(glm::vec3(v, v, 1), e3::ETransformAlignment::Center);
                }, [this](){


                });
                {
                    e3::Animation *a = new e3::Animation();
                    a->Start(0.5, e3::EAnimation::EaseOutBack, [this](float v) {
                        mLogo->SetOpacity(1);
                        mLogo->SetScale(glm::vec3(v, v, 1), e3::ETransformAlignment::Center);
                    }, []() {

                    });
                }
            }
        });
    }


}

void Splash::Animate(std::string text, int i)
{
    if (i == text.size()) return;
    char c = text[i];
    PText* t = new PText();
    t->SetFont("Picsart/Twenty Minutes.otf");
    t->SetFontSize("40dp");
    t->SetFontStyle(e3::EFontStyle::Bold);
    t->SetPositionType(e3::EPositionType::Absolute);
    t->SetTextColor(glm::vec4(0, 0, 0, 255));
    t->SetText(std::string(1, c));
    mTextCont->AddElement(t);

    e3::Animation* a = new e3::Animation();
    a->Start(0.1, [t, i, text](float v){
        t->SetScale(glm::vec3(v), e3::ETransformAlignment::Center);
        t->SetTranslation(glm::vec3(-v * (text.size() - i - 1) * e3::Dim("20dp"), 0, 0));
    }, [this, t, text, i](){

//        e3::Animation* a = new e3::Animation();
//        a->Start(0.2, e3::EAnimation::EaseOutBack, [t, i](float v){
//            t->SetTranslation(glm::vec3(v * i * e3::Dim("20dp"), 0, 0));
//        }, [this, text, i](){
            Animate(text, i+1);
//        });

    });
}
