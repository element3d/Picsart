#include "ImageCanvas.h"
#include <e3/Application.h>
ImageCanvas::ImageCanvas(e3::Element* pParent)
	: ImageCanvasBase(pParent)
{
    float pVertices[] =
    {
       -0.5, -0.5, 0, 0,
       0.5, -0.5, 1, 0,
       -0.5, 0.5, 0, 1,
       -0.5, 0.5, 0, 1,
       0.5, -0.5, 1, 0,
       0.5, 0.5, 1, 1
    };
    mVertexBuffer = new e3::GLBuffer(e3::EGLBufferType::Vertex, sizeof(pVertices), pVertices);
}

void ImageCanvas::SetFilter(EFilter filter)
{
    if (mFilter) delete mFilter;
    mFilter = nullptr;
    if (mProgram) delete mProgram;
    mProgram = nullptr;
    mFilterType = filter;
    _CreateData();
}

void ImageCanvas::_CreateData()
{

    std::string vs = R"(
    <E3_GLSL_VERSION_CODE>

    precision mediump float;

    layout(location = 0) in vec2 aPos;
    layout(location = 1) in vec2 aUV;

    uniform mat4 proj;
    uniform vec4 uvt;

    out vec2 oUV;

    void main()
    {
        oUV.x = aUV.x * uvt.x;
        oUV.y = aUV.y * uvt.y;
        oUV += uvt.zw;
//        oUV.y = -aUV.y;
        gl_Position = proj * vec4(aPos, 0.0, 1.0);
    }
        )";

    if (mFilterType == EFilter::Toaster)
    {
        mFilter = new e3::Image();
        mFilter->LoadAsync(GetApplication()->GetAssetManager()->GetAssetPath("Picsart/toaster.png"), new e3::OnLoadCallback([this](){
            int w = mFilter->GetWidth();
            void* d = mFilter->GetData();
        }));

            std::string fs = R"(
        <E3_GLSL_VERSION_CODE>
        precision mediump float;
        in vec2 oUV;
        out vec4 FragColor;
        uniform sampler2D u_Texture0;
        uniform sampler2D u_Texture4;

        uniform vec4 uTextureColor;
    const vec3 W = vec3(0.2125, 0.7154, 0.0721);

    vec3 BrightnessContrastSaturation(vec3 color, float brt, float con, float sat)
    {
        vec3 black = vec3(0., 0., 0.);
        vec3 middle = vec3(0.5, 0.5, 0.5);
        float luminance = dot(color, W);
        vec3 gray = vec3(luminance, luminance, luminance);

        vec3 brtColor = mix(black, color, brt);
        vec3 conColor = mix(middle, brtColor, con);
        vec3 satColor = mix(gray, conColor, sat);
        return satColor;
    }

    vec3 ovelayBlender(vec3 Color, vec3 filterr){
        vec3 filter_result;

        //if(luminance < 0.5)
        //	filter_result = 2. * trans_filter * Color;
        //else
            filter_result = 1. - (1. - (2. *(filterr - 0.5)))*(1. - Color);

        return filter_result;
    }

        void main()
        {
     vec2 st = oUV.st;
         vec3 irgb = texture(u_Texture0, st).rgb;
         vec3 filterr = texture(u_Texture4, st).rgb;

         //adjust the brightness/contrast/saturation
         float T_bright = 1.0;
         float T_contrast = 1.0;
         float T_saturation = 1.0;
         vec3 bcs_result = BrightnessContrastSaturation(irgb, T_bright, T_contrast, T_saturation);

         //more red
         vec3 rb_result = vec3(bcs_result.r*1.3, bcs_result.g, bcs_result.b*0.9);

         //add filter (overlay blending)
         vec3 after_filter = mix(bcs_result, ovelayBlender(bcs_result, filterr), 0.55);

         FragColor = vec4(after_filter, 1.);
        }
            )";

        Carbon::ShaderProgramCreateInfo2 info;
        info.VsCode = vs;
        info.FsCode = fs;
        info.Version = Carbon::EShaderVersion::GLSL_300_ES;
        mProgram = new Carbon::ShaderProgram(&info);
    }
    else if (mFilterType == EFilter::Rise)
    {
        mFilter = new e3::Image();
        mFilter->LoadAsync(GetApplication()->GetAssetManager()->GetAssetPath("Picsart/filter.png"), new e3::OnLoadCallback([this](){
            int w = mFilter->GetWidth();
            void* d = mFilter->GetData();
        }));

            std::string fs = R"(
        <E3_GLSL_VERSION_CODE>
        precision mediump float;
        in vec2 oUV;
        out vec4 FragColor;
        uniform sampler2D u_Texture0;
        uniform sampler2D u_Texture4;

        uniform vec4 uTextureColor;
    const vec3 W = vec3(0.2125, 0.7154, 0.0721);


vec3 BrightnessContrastSaturation(vec3 color, float brt, float con, float sat)
{
    vec3 black = vec3(0., 0., 0.);
    vec3 middle = vec3(0.5, 0.5, 0.5);
    float luminance = dot(color, W);
    vec3 gray = vec3(luminance, luminance, luminance);

    vec3 brtColor = mix(black, color, brt);
    vec3 conColor = mix(middle, brtColor, con);
    vec3 satColor = mix(gray, conColor, sat);
    return satColor;
}

vec3 ovelayBlender(vec3 Color, vec3 filterr){
    vec3 filter_result;

    float luminance = dot(filterr, W);

    if(luminance < 0.5)
        filter_result = 2. * filterr * Color;
    else
        filter_result = 1. - (1. - (2. *(filterr - 0.5)))*(1. - Color);

    return filter_result;
}

void main()
{
     //get the pixel
     vec2 st = oUV.st;
     vec3 irgb = texture(u_Texture0, st).rgb;
     vec3 filterr = texture(u_Texture4, st).rgb;

     //adjust the brightness/contrast/saturation
     float T_bright = 1.2;
     float T_contrast = 1.0;
     float T_saturation = 0.9;
     vec3 bcs_result = BrightnessContrastSaturation(irgb, T_bright, T_contrast, T_saturation);

     //more red, less blue
     vec3 rb_result = vec3(bcs_result.r*1.1, bcs_result.g, bcs_result.b*0.9);

     //add filter (overlay blending)
     vec3 after_filter = mix(rb_result, ovelayBlender(rb_result, filterr), 0.5);

     FragColor = vec4(after_filter, 1.);
}
            )";

        Carbon::ShaderProgramCreateInfo2 info;
        info.VsCode = vs;
        info.FsCode = fs;
        info.Version = Carbon::EShaderVersion::GLSL_300_ES;
        mProgram = new Carbon::ShaderProgram(&info);
    }

    else if (mFilterType == EFilter::Willow)
    {
            std::string fs = R"(
        <E3_GLSL_VERSION_CODE>
        precision mediump float;
        in vec2 oUV;
        out vec4 FragColor;
        uniform sampler2D u_Texture0;

        uniform vec4 uTextureColor;
    const vec3 W = vec3(0.2125, 0.7154, 0.0721);


vec3 BrightnessContrastSaturation(vec3 color, float brt, float con, float sat)
{
    vec3 black = vec3(0., 0., 0.);
    vec3 middle = vec3(0.5, 0.5, 0.5);

    vec3 brtColor = mix(black, color, brt);
    vec3 conColor = mix(middle, brtColor, con);
    vec3 satColor = mix(color, conColor, sat);
    return satColor;
}

void main()
{

     const vec3 W = vec3(0.2125, 0.1754, 0.0721);

     vec3 irgb = texture(u_Texture0, oUV).rgb;

     float luminance = dot(irgb, W);
     vec3 gray = vec3(luminance, luminance, luminance);
     vec3 result = BrightnessContrastSaturation(gray, 2.5, 1.1, 1.0);

     FragColor = vec4(result, 1.);
}
            )";

        Carbon::ShaderProgramCreateInfo2 info;
        info.VsCode = vs;
        info.FsCode = fs;
        info.Version = Carbon::EShaderVersion::GLSL_300_ES;
        mProgram = new Carbon::ShaderProgram(&info);
    }

    else if (mFilterType == EFilter::XPro)
    {
        mFilter = new e3::Image();
        mFilter->LoadAsync(GetApplication()->GetAssetManager()->GetAssetPath("Picsart/filter.png"), new e3::OnLoadCallback([this](){
            int w = mFilter->GetWidth();
            void* d = mFilter->GetData();
        }));

            std::string fs = R"(
        <E3_GLSL_VERSION_CODE>
        precision mediump float;
        in vec2 oUV;
        out vec4 FragColor;
        uniform sampler2D u_Texture0;
        uniform sampler2D u_Texture4;

        uniform vec4 uTextureColor;
    const vec3 W = vec3(0.2125, 0.7154, 0.0721);

vec3 BrightnessContrastSaturation(vec3 color, float brt, float con, float sat)
{
    vec3 black = vec3(0., 0., 0.);
    vec3 middle = vec3(0.5, 0.5, 0.5);
    float luminance = dot(color, W);
    vec3 gray = vec3(luminance, luminance, luminance);

    vec3 brtColor = mix(black, color, brt);
    vec3 conColor = mix(middle, brtColor, con);
    vec3 satColor = mix(gray, conColor, sat);
    return satColor;
}

vec3 multiplyBlender(vec3 Color, vec3 filterr){
    vec3 filter_result;
    float luminance = dot(filterr, W);

    if(luminance < 0.5)
        filter_result = 2. * filterr * Color;
    else
        filter_result = Color;

    return filter_result;
}

vec3 ovelayBlender(vec3 Color, vec3 filterr){
    vec3 filter_result;

    float luminance = dot(filterr, W);

    if(luminance < 0.5)
        filter_result = 2. * filterr * Color;
    else
        filter_result = 1. - (1. - (2. *(filterr - 0.5)))*(1. - Color);

    return filter_result;
}

void main()
{
     //get the pixel
     vec2 st = oUV.st;
     vec3 irgb = texture(u_Texture0, st).rgb;
     vec3 filterr = texture(u_Texture4, st).rgb;

     //adjust the brightness/contrast/saturation
     float T_bright = 1.2;
     float T_contrast = 1.2;
     float T_saturation = 1.3;
     vec3 bcs_result = BrightnessContrastSaturation(irgb, T_bright, T_contrast, T_saturation);


     //add filter (overlay blending)
     vec3 after_filter = mix(bcs_result, multiplyBlender(bcs_result, filterr), 0.7);

     FragColor = vec4(after_filter, 1.);
}

            )";

        Carbon::ShaderProgramCreateInfo2 info;
        info.VsCode = vs;
        info.FsCode = fs;
        info.Version = Carbon::EShaderVersion::GLSL_300_ES;
        mProgram = new Carbon::ShaderProgram(&info);
    }


    else if (mFilterType == EFilter::Mayfair)
    {
        mFilter = new e3::Image();
        mFilter->LoadAsync(GetApplication()->GetAssetManager()->GetAssetPath("Picsart/filter.png"), new e3::OnLoadCallback([this](){
            int w = mFilter->GetWidth();
            void* d = mFilter->GetData();
        }));

            std::string fs = R"(
        <E3_GLSL_VERSION_CODE>
        precision mediump float;
        in vec2 oUV;
        out vec4 FragColor;
        uniform sampler2D u_Texture0;
        uniform sampler2D u_Texture4;

        uniform vec4 uTextureColor;
    const vec3 W = vec3(0.2125, 0.7154, 0.0721);

vec3 BrightnessContrastSaturation(vec3 color, float brt, float con, float sat)
{
    vec3 black = vec3(0., 0., 0.);
    vec3 middle = vec3(0.5, 0.5, 0.5);
    float luminance = dot(color, W);
    vec3 gray = vec3(luminance, luminance, luminance);

    vec3 brtColor = mix(black, color, brt);
    vec3 conColor = mix(middle, brtColor, con);
    vec3 satColor = mix(gray, conColor, sat);
    return satColor;
}

vec3 ovelayBlender(vec3 Color, vec3 filterr){
    vec3 filter_result;
    float luminance = dot(filterr, W);

    if(luminance < 0.5)
        filter_result = 2. * filterr * Color;
    else
        filter_result = 1. - (1. - (2. *(filterr - 0.5)))*(1. - Color);

    return filter_result;
}

void main()
{
     //get the pixel
     vec2 st = oUV.st;
     vec3 irgb = texture(u_Texture0, st).rgb;
     vec3 filterr = texture(u_Texture4, st).rgb;

     //adjust the brightness/contrast/saturation
     float T_bright = 1.1;
     float T_contrast = 1.1;
     float T_saturation = 1.3;
     vec3 bcs_result = BrightnessContrastSaturation(irgb, T_bright, T_contrast, T_saturation);

     //add filter (overlay blending)
     vec3 after_filter = mix(bcs_result, ovelayBlender(bcs_result, filterr), 0.3);

     FragColor = vec4(after_filter, 1.);
}
            )";

        Carbon::ShaderProgramCreateInfo2 info;
        info.VsCode = vs;
        info.FsCode = fs;
        info.Version = Carbon::EShaderVersion::GLSL_300_ES;
        mProgram = new Carbon::ShaderProgram(&info);
    }

    else if (mFilterType == EFilter::Hudson)
    {
        mFilter = new e3::Image();
        mFilter->LoadAsync(GetApplication()->GetAssetManager()->GetAssetPath("Picsart/hudson.png"), new e3::OnLoadCallback([this](){
            int w = mFilter->GetWidth();
            void* d = mFilter->GetData();
        }));

            std::string fs = R"(
        <E3_GLSL_VERSION_CODE>
        precision mediump float;
        in vec2 oUV;
        out vec4 FragColor;
        uniform sampler2D u_Texture0;
        uniform sampler2D u_Texture4;

        uniform vec4 uTextureColor;
    const vec3 W = vec3(0.2125, 0.7154, 0.0721);

vec3 BrightnessContrastSaturation(vec3 color, float brt, float con, float sat)
{
    vec3 black = vec3(0., 0., 0.);
    vec3 middle = vec3(0.5, 0.5, 0.5);
    float luminance = dot(color, W);
    vec3 gray = vec3(luminance, luminance, luminance);

    vec3 brtColor = mix(black, color, brt);
    vec3 conColor = mix(middle, brtColor, con);
    vec3 satColor = mix(gray, conColor, sat);
    return satColor;
}

vec3 ovelayBlender(vec3 Color, vec3 filterr){
    vec3 filter_result;
    float luminance = dot(filterr, W);

    if(luminance < 0.5)
        filter_result = 2. * filterr * Color;
    else
        filter_result = 1. - (1. - (2. *(filterr - 0.5)))*(1. - Color);

    return filter_result;
}

void main()
{
     //get the pixel
     vec2 st = oUV.st;
     vec3 irgb = texture(u_Texture0, st).rgb;
     vec3 filterr = texture(u_Texture4, st).rgb;

     //adjust the brightness/contrast/saturation
     float T_bright = 1.2;
     float T_contrast = 1.1;
     float T_saturation = 0.9;
     vec3 bcs_result = BrightnessContrastSaturation(irgb, T_bright, T_contrast, T_saturation);



     //add filter (overlay blending)
     vec3 after_filter = mix(bcs_result, ovelayBlender(bcs_result, filterr), 0.4);

     FragColor = vec4(after_filter, 1.);
}
            )";

        Carbon::ShaderProgramCreateInfo2 info;
        info.VsCode = vs;
        info.FsCode = fs;
        info.Version = Carbon::EShaderVersion::GLSL_300_ES;
        mProgram = new Carbon::ShaderProgram(&info);
    }


    else if (mFilterType == EFilter::Hefe)
    {
        mFilter = new e3::Image();
        mFilter->LoadAsync(GetApplication()->GetAssetManager()->GetAssetPath("Picsart/hefe.png"), new e3::OnLoadCallback([this](){
            int w = mFilter->GetWidth();
            void* d = mFilter->GetData();
        }));

            std::string fs = R"(
        <E3_GLSL_VERSION_CODE>
        precision mediump float;
        in vec2 oUV;
        out vec4 FragColor;
        uniform sampler2D u_Texture0;
        uniform sampler2D u_Texture4;

        uniform vec4 uTextureColor;
    const vec3 W = vec3(0.2125, 0.7154, 0.0721);

vec3 BrightnessContrastSaturation(vec3 color, float brt, float con, float sat)
{
    vec3 black = vec3(0., 0., 0.);
    vec3 middle = vec3(0.5, 0.5, 0.5);
    float luminance = dot(color, W);
    vec3 gray = vec3(luminance, luminance, luminance);

    vec3 brtColor = mix(black, color, brt);
    vec3 conColor = mix(middle, brtColor, con);
    vec3 satColor = mix(gray, conColor, sat);
    return satColor;
}

vec3 ovelayBlender(vec3 Color, vec3 filterr){
    vec3 filter_result;
    float luminance = dot(filterr, W);

    if(luminance < 0.5)
        filter_result = 2. * filterr * Color;
    else
        filter_result = 1. - (1. - (2. *(filterr - 0.5)))*(1. - Color);

    return filter_result;
}

void main()
{
     //get the pixel
     vec2 st = oUV.st;
     vec3 irgb = texture(u_Texture0, st).rgb;
     vec3 filterr = texture(u_Texture4, st).rgb;

     //adjust the brightness/contrast/saturation
     float T_bright = 1.3;
     float T_contrast = 1.0;
     float T_saturation = 1.3;
     vec3 bcs_result = BrightnessContrastSaturation(irgb, T_bright, T_contrast, T_saturation);

     //more red, less blue
     vec3 rb_result = vec3(bcs_result.r*1.15, bcs_result.g, bcs_result.b*0.8);

     //add filter (overlay blending)
     vec3 after_filter = mix(rb_result, ovelayBlender(rb_result, filterr), 0.8);

     FragColor = vec4(after_filter, 1.);
}
            )";

        Carbon::ShaderProgramCreateInfo2 info;
        info.VsCode = vs;
        info.FsCode = fs;
        info.Version = Carbon::EShaderVersion::GLSL_300_ES;
        mProgram = new Carbon::ShaderProgram(&info);
    }

    else if (mFilterType == EFilter::EarlyBird)
    {
        mFilter = new e3::Image();
        mFilter->LoadAsync(GetApplication()->GetAssetManager()->GetAssetPath("Picsart/filter.png"), new e3::OnLoadCallback([this](){
            int w = mFilter->GetWidth();
            void* d = mFilter->GetData();
        }));

            std::string fs = R"(
        <E3_GLSL_VERSION_CODE>
        precision mediump float;
        in vec2 oUV;
        out vec4 FragColor;
        uniform sampler2D u_Texture0;
        uniform sampler2D u_Texture4;

        uniform vec4 uTextureColor;
    const vec3 W = vec3(0.2125, 0.7154, 0.0721);

vec3 BrightnessContrastSaturation(vec3 color, float brt, float con, float sat)
{
    vec3 black = vec3(0., 0., 0.);
    vec3 middle = vec3(0.5, 0.5, 0.5);
    float luminance = dot(color, W);
    vec3 gray = vec3(luminance, luminance, luminance);

    vec3 brtColor = mix(black, color, brt);
    vec3 conColor = mix(middle, brtColor, con);
    vec3 satColor = mix(gray, conColor, sat);
    return satColor;
}

vec3 ovelayBlender(vec3 Color, vec3 filterr){
    vec3 filter_result;
    float luminance = dot(filterr, W);

    if(luminance < 0.5)
        filter_result = 2. * filterr * Color;
    else
        filter_result = 1. - (1. - (2. *(filterr - 0.5)))*(1. - Color);

    return filter_result;
}

vec3 multiplyBlender(vec3 Color, vec3 filterr){
    vec3 filter_result;
    float luminance = dot(filterr, W);

    if(luminance < 0.5)
        filter_result = 2. * filterr * Color;
    else
        filter_result = Color;

    return filter_result;
}

void main()
{
     //get the pixel
     vec2 st = oUV.st;
     vec3 irgb = texture(u_Texture0, st).rgb;
     vec3 filterr = texture(u_Texture4, st).rgb;

     //adjust the brightness/contrast/saturation
     float T_bright = 1.2;
     float T_contrast = 1.1;
     float T_saturation = 1.2;
     vec3 bcs_result = BrightnessContrastSaturation(irgb, T_bright, T_contrast, T_saturation);

     //more red, less blue
     vec3 rb_result = vec3(bcs_result.r*1.1, bcs_result.g, bcs_result.b*0.9);

     //add filter (overlay blending)
     vec3 after_filter = mix(rb_result, multiplyBlender(rb_result, filterr), 0.8);

     FragColor = vec4(after_filter, 1.);
}
            )";

        Carbon::ShaderProgramCreateInfo2 info;
        info.VsCode = vs;
        info.FsCode = fs;
        info.Version = Carbon::EShaderVersion::GLSL_300_ES;
        mProgram = new Carbon::ShaderProgram(&info);
    }

    else if (mFilterType == EFilter::Amaro)
    {
        mFilter = new e3::Image();
        mFilter->LoadAsync(GetApplication()->GetAssetManager()->GetAssetPath("Picsart/filter.png"), new e3::OnLoadCallback([this](){
            int w = mFilter->GetWidth();
            void* d = mFilter->GetData();
        }));

            std::string fs = R"(
        <E3_GLSL_VERSION_CODE>
        precision mediump float;
        in vec2 oUV;
        out vec4 FragColor;
        uniform sampler2D u_Texture0;
        uniform sampler2D u_Texture4;

        uniform vec4 uTextureColor;
    const vec3 W = vec3(0.2125, 0.7154, 0.0721);

vec3 BrightnessContrastSaturation(vec3 color, float brt, float con, float sat)
{
    vec3 black = vec3(0., 0., 0.);
    vec3 middle = vec3(0.5, 0.5, 0.5);
    float luminance = dot(color, W);
    vec3 gray = vec3(luminance, luminance, luminance);

    vec3 brtColor = mix(black, color, brt);
    vec3 conColor = mix(middle, brtColor, con);
    vec3 satColor = mix(gray, conColor, sat);
    return satColor;
}

vec3 ovelayBlender(vec3 Color, vec3 filterr){
    vec3 filter_result;
    float luminance = dot(filterr, W);

    if(luminance < 0.5)
        filter_result = 2. * filterr * Color;
    else
        filter_result = 1. - (1. - (2. *(filterr - 0.5)))*(1. - Color);

    return filter_result;
}

void main()
{
     //get the pixel
     vec2 st = oUV.st;
     vec3 irgb = texture(u_Texture0, st).rgb;
     vec3 filterr = texture(u_Texture4, st).rgb;

     //adjust the brightness/contrast/saturation
     float T_bright = 1.1;
     float T_contrast = 1.1;
     float T_saturation = 1.1;
     vec3 bcs_result = BrightnessContrastSaturation(irgb, T_bright, T_contrast, T_saturation);

     //add blue
     vec3 blue_result = vec3(bcs_result.r, bcs_result.g*1.03, bcs_result.b);

     //add filter (overlay blending)
     vec3 after_filter = mix(blue_result, ovelayBlender(blue_result, filterr), 0.6);

     FragColor = vec4(after_filter, 1.);
}
            )";

        Carbon::ShaderProgramCreateInfo2 info;
        info.VsCode = vs;
        info.FsCode = fs;
        info.Version = Carbon::EShaderVersion::GLSL_300_ES;
        mProgram = new Carbon::ShaderProgram(&info);
    }
}

void ImageCanvas::OnMouseWhell(e3::MouseEvent* e)
{
    mScale += e->GetDistanceY();
    SetScale(glm::vec3(mScale), e3::ETransformAlignment::Center);
}

bool ImageCanvas:: OnScale(e3::ScaleEvent* e)
{
    if (e->GetScaleFactor() > 1.001) mScale += 0.2;
    else if (e->GetScaleFactor() < 0.98) mScale -= 0.2;

    SetScale(glm::vec3(mScale), e3::ETransformAlignment::Center);
    return ImageCanvasBase::OnScale(e);
}

void ImageCanvas::Render() 
{
    ImageCanvasBase::Render();

//    glScissor( 0, 0, GetGeometry().width, GetGeometry().height);

auto e = glGetError();
    mVertexBuffer->bind();
    mVertexBuffer->vertexAttribPointer(0, 2, 4 * sizeof(float), 0);
    mVertexBuffer->vertexAttribPointer(1, 2, 4 * sizeof(float), 2 * sizeof(float));
    mProgram->bind();

    if (!mImage || mImage->empty()) return;

    if (mImage && !mImage->empty()) {
        glActiveTexture(GL_TEXTURE0);
        mProgram->setInt("u_Texture0", 0);
        mImage->GetTexture()->bind();
    }

    if (mFilter && !mFilter->empty())
    {
        glActiveTexture(GL_TEXTURE1);
        mProgram->setInt("u_Texture4", 1);
        mFilter->GetTexture()->bind();
    }

    auto geo = GetGeometry();

    float V = (float)mImage->GetWidth() / mImage->GetHeight();
    float A = geo.width / geo.height;
    float aspectRatioWidth = A >= V ? V / A : 1;
    float aspectRatioHeight = V > A ? A / V : 1;
    glm::vec4 mImageUVTransform = glm::vec4(1, 1, 0, 0);
  /*  if (mImageFit == EImageFit::Cover)
    {
        float scale = 1.0;
        if (mImagePosition == EImagePosition::Start)
            scale = 0.0;
        else if (mImagePosition == EImagePosition::Center)
            scale = 0.5;
        else
            scale = 1.0;

        if (aspectRatioHeight != 1)
        {
            mImageUVTransform.x = aspectRatioHeight;//(1.0 - aspectRatioHeight) / 2.0;
            mImageUVTransform.z = (1.0 - aspectRatioHeight) * scale;
        }
        if (aspectRatioWidth != 1.0)
        {
            mImageUVTransform.y = aspectRatioWidth;// (1.0 - aspectRatioWidth) / 2.0;
            mImageUVTransform.w = (1.0 - aspectRatioWidth) * scale;
        }
    }
    else if (mImageFit == EImageFit::Contain)*/
    {
        float scale = 1;
        /*if (mImagePosition == EImagePosition::Start)
            scale = 0.0;
        else if (mImagePosition == EImagePosition::Center)*/
            scale = 0.5;
        /*else
            scale = 1.0;*/
        if (aspectRatioHeight != 1)
        {
            mImageUVTransform.y = 1 / aspectRatioHeight;//(1.0 - aspectRatioHeight) / 2.0;
            mImageUVTransform.w = -(mImageUVTransform.y - 1)*scale;
        }
        if (aspectRatioWidth != 1.0)
        {
            mImageUVTransform.x = 1 / aspectRatioWidth;// (1.0 - aspectRatioWidth) / 2.0;
            mImageUVTransform.z = -(mImageUVTransform.x - 1)*scale;//-(1.0 - aspectRatioWidth/2) * scale;
        }
    }

//    glm::vec2 mImageUVTransform(1);
//    if (!mImage->empty())
//    {
//        if (mImage->GetHeight() / geo.height > mImage->GetWidth() / geo.width)
//        {
////            mImageUVTransform.x = mTexture->mHeight / mTexture->mWidth;
//             mImageUVTransform.x = (mImage->GetWidth() * geo.height) / (mImage->GetHeight() * geo.width);
//        }
//        else
//        {
//            mImageUVTransform.y = mImage->GetWidth() / mImage->GetHeight();
////            if (mImagePosition == EImagePosition::Start)
////                mImageUVTransform.w = (1 - mImageUVTransform.y) / 1.0;
////            else if (mImagePosition == EImagePosition::Center)
////                mImageUVTransform.w = (1 - mImageUVTransform.y) / 2.0;
//        }
//    }
    auto s = GetApplication()->GetWindowSize();

    glm::mat4 projection = glm::ortho(0.0f, (GLfloat)s.Width, (GLfloat)s.Height, 0.0f, -1.0f, 1.0f);
    glm::mat4 scale = glm::scale(glm::mat4(1), glm::vec3(geo.width / 1, geo.height / 1, 1.0));
    glm::mat4 ss = glm::scale(glm::mat4(1), glm::vec3(mScale));
    glm::mat4 translate = glm::translate(glm::mat4(1), glm::vec3(geo.x + geo.width / 2.0, geo.y + geo.height / 2, 0));
   mProgram->setMat4("proj", projection * translate * scale);
    mProgram->setVec4("uvt", mImageUVTransform);
//   glViewport(geo.x, GetApplication()->GetWindowSize().Height - geo.height -geo.y, geo.width, geo.height);
    glDrawArrays(GL_TRIANGLES, 0, 6);
//    glViewport(0, 0, s.Width, s.Height);
    auto e1 = glGetError();
auto e2= glGetError();
}
