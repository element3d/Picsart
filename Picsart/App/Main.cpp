#include "Main.h"
#include <e3/Application.h>
#include "MainFilterItem.h"
Main::Main(e3::Element* pParent)
	: MainBase(pParent)
{
    e3::Image* mImage = new e3::Image();
    mImage->LoadAsync(GetApplication()->GetAssetManager()->GetAssetPath("Picsart/picture1.jpeg"), new e3::OnLoadCallback([this](){
//        int w = mImage->GetWidth();
//        void* d = mImage->GetData();
    }));

    e3::Image* mImage2 = new e3::Image();
    mImage2->LoadAsync(GetApplication()->GetAssetManager()->GetAssetPath("Picsart/image2.jpg"), new e3::OnLoadCallback([this](){
//        int w = mImage->GetWidth();
//        void* d = mImage->GetData();
    }));

    e3::Image* mImage3 = new e3::Image();
    mImage3->LoadAsync(GetApplication()->GetAssetManager()->GetAssetPath("Picsart/image3.jpg"), new e3::OnLoadCallback([this](){
//        int w = mImage->GetWidth();
//        void* d = mImage->GetData();
    }));


    {
        e3::Element* i = new e3::Element();
        i->SetHeight("100%");
        i->SetAspectRatio(1);
        i->SetMargin("10dp");
        i->SetBackgroundImageFit(e3::EBackgroundSize::Cover);
        i->SetBackgroundImage(std::shared_ptr<e3::Image>(mImage));
        mImagesCont->AddElement(i);
    }

    {
        e3::Element* i = new e3::Element();
        i->SetHeight("100%");
        i->SetAspectRatio(1);
        i->SetMargin("10dp");
        i->SetBackgroundImageFit(e3::EBackgroundSize::Cover);
        i->SetBackgroundImage(std::shared_ptr<e3::Image>(mImage2));
        mImagesCont->AddElement(i);
    }

    {
        e3::Element* i = new e3::Element();
        i->SetHeight("100%");
        i->SetAspectRatio(1);
        i->SetMargin("10dp");
        i->SetBackgroundImageFit(e3::EBackgroundSize::Cover);
        i->SetBackgroundImage(std::shared_ptr<e3::Image>(mImage3));
        mImagesCont->AddElement(i);
    }

    for (int i = 0; i < mImagesCont->GetNumChildren(); ++i)
    {
        mImagesCont->GetChildren()[i]->SetOnClickCallback([this, i](e3::MouseEvent*){
            mCanvas->SetImage(mImagesCont->GetChildren()[i]->GetBackgroundImage().get());

            for (int ii = 0; ii < int(EFilter::NumFilters); ++ii)
            {
                MainFilterItem* f = (MainFilterItem*)mFilters->GetChildren()[ii];
                f->GetCanvas()->SetImage(mImagesCont->GetChildren()[i]->GetBackgroundImage().get());
            }
        });
    }

    mCanvas->SetImage(mImage);
    mCanvas->SetFilter(EFilter::Hefe);

    auto fFilterToString = [](EFilter filter){
        switch (filter) {
        case EFilter::Amaro:
            return "Amaro";
        case EFilter::EarlyBird:
            return "Early bird";
        case EFilter::Hefe:
            return "Hefe";
        case EFilter::Hudson:
            return "Hudson";
        case EFilter::Mayfair:
            return "Mayfair";
        case EFilter::Rise:
            return "Rise";
        case EFilter::Toaster:
            return "Toaster";
        case EFilter::Willow:
            return "Willow";
        case EFilter::XPro:
            return "XPro";

        }
        return "";
    };

    for (int i = 0; i < int(EFilter::NumFilters); ++i)
    {
        MainFilterItem* c = new MainFilterItem();
//        c->SetWidth("70dp");
//        c->SetHeight("96%");
//        c->SetMargin("5dp");
        c->GetCanvas()->SetImage(mImage);
        c->GetCanvas()->SetFilter(EFilter(i));
        mFilters->AddElement(c);
        c->SetTitle(fFilterToString(EFilter(i)));
        if (i == 0) c->Check();

        c->SetOnClickCallback([this, i, c](e3::MouseEvent*){
            for (int i = 0; i < int(EFilter::NumFilters); ++i)
            {
                MainFilterItem* cc = (MainFilterItem*)mFilters->GetChildren()[i];
                cc->Uncheck();
            }

            c->Check();
            mCanvas->SetFilter(EFilter(i));
        });
    }



    mCanvasCont->SetOnScaleCallback([](e3::ScaleEvent* pEvent){
        float s = pEvent->GetScaleFactor();

    });
}
