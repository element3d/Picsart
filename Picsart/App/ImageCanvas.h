
#ifndef ImageCanvas_H_
#define ImageCanvas_H_

#include "private/ImageCanvasBase.h"
#include <e3/GLBuffer.h>
#include <e3/GLShaderProgram.h>

enum class EFilter
{
    Rise,
    Toaster,
    Willow,
    XPro,
    Mayfair,
    Hudson,
    Hefe,
    EarlyBird,
    Amaro,
    NumFilters
};

class E3_EXPORT ImageCanvas : public ImageCanvasBase
{
public:
	ImageCanvas(e3::Element* pParent = nullptr);
    void SetImage(e3::Image* pImage) { mImage = pImage; }
    void SetFilter(EFilter filter);

	virtual void Render() override;

    virtual void OnMouseWhell(e3::MouseEvent*) override;
	virtual bool OnScale(e3::ScaleEvent*) override;
private:
    void _CreateData();

private:
    float mScale = 1;
    e3::GLBuffer* mVertexBuffer;
    Carbon::ShaderProgram* mProgram = nullptr;
    e3::Image* mFilter = nullptr;
    e3::Image* mImage = nullptr;
    EFilter mFilterType;
};

#endif // ImageCanvas_H_

