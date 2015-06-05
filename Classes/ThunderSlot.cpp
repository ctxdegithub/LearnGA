//
//  TunderSlot.cpp
//  LearnGA
//
//  Created by DONG on 15/5/14.
//
//

#include "ThunderSlot.h"

ThunderSlot::ThunderSlot()
: _fastMode(false)
, _startingPositionInitialized(false)
, _texture(nullptr)
, _blendFunc(BlendFunc::ALPHA_NON_PREMULTIPLIED)
, _stroke(0.0f)
, _fadeDelta(0.0f)
, _minSeg(0.0f)
, _angle(0.0f)
, _maxPoints(0)
, _nuPoints(0)
, _previousNuPoints(0)
, _pointVertexes(nullptr)
, _pointState(nullptr)
, _vertices(nullptr)
, _colorPointer(nullptr)
, _texCoords(nullptr)
{
}

ThunderSlot::~ThunderSlot()
{
    CC_SAFE_RELEASE(_texture);
    CC_SAFE_FREE(_pointState);
    CC_SAFE_FREE(_pointVertexes);
    CC_SAFE_FREE(_vertices);
    CC_SAFE_FREE(_colorPointer);
    CC_SAFE_FREE(_texCoords);
}

ThunderSlot* ThunderSlot::create(float fade, float minSeg, float stroke, const Color3B& color, const std::string& path)
{
    ThunderSlot *ret = new (std::nothrow) ThunderSlot();
    if (ret && ret->initWithFade(fade, minSeg, stroke, color, path))
    {
        ret->autorelease();
        return ret;
    }
    
    CC_SAFE_DELETE(ret);
    return nullptr;
}

ThunderSlot* ThunderSlot::create(float fade, float minSeg, float stroke, const Color3B& color, Texture2D* texture)
{
    ThunderSlot *ret = new (std::nothrow) ThunderSlot();
    if (ret && ret->initWithFade(fade, minSeg, stroke, color, texture))
    {
        ret->autorelease();
        return ret;
    }
    
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool ThunderSlot::initWithFade(float fade, float minSeg, float stroke, const Color3B& color, const std::string& path)
{
    CCASSERT(!path.empty(), "Invalid filename");
    
    Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(path);
    return initWithFade(fade, minSeg, stroke, color, texture);
}

bool ThunderSlot::initWithFade(float fade, float minSeg, float stroke, const Color3B& color, Texture2D* texture)
{
    Node::setPosition(Vec2::ZERO);
    setAnchorPoint(Vec2::ZERO);
    ignoreAnchorPointForPosition(true);
    _startingPositionInitialized = false;
    
    _positionR = Vec2::ZERO;
    _fastMode = true;
    _minSeg = (minSeg == -1.0f) ? stroke/5.0f : minSeg;
    _minSeg *= _minSeg;
    
    _stroke = stroke;
    _fadeDelta = 1.0f/fade;
    
    _maxPoints = (int)(fade*60.0f)+2;
    _nuPoints = 0;
    _pointState = (float *)malloc(sizeof(float) * _maxPoints);
    _pointVertexes = (Vec2*)malloc(sizeof(Vec2) * _maxPoints);
    
    _vertices = (Vec2*)malloc(sizeof(Vec2) * _maxPoints * 2);
    _texCoords = (Tex2F*)malloc(sizeof(Tex2F) * _maxPoints * 2);
    _colorPointer =  (GLubyte*)malloc(sizeof(GLubyte) * _maxPoints * 2 * 4);
    
    // Set blend mode
    _blendFunc = BlendFunc::ALPHA_NON_PREMULTIPLIED;
    
    // shader state
    setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR));
    
    setTexture(texture);
    setColor(color);
    scheduleUpdate();
    
    return true;
}

void ThunderSlot::setPosition(const Vec2& position)
{
    if (!_startingPositionInitialized) {
        _startingPositionInitialized = true;
    }
    _positionR = position;
}

void ThunderSlot::setPosition(float x, float y)
{
    if (!_startingPositionInitialized) {
        _startingPositionInitialized = true;
    }
    _positionR.x = x;
    _positionR.y = y;
}

const Vec2& ThunderSlot::getPosition() const
{
    return _positionR;
}

void ThunderSlot::getPosition(float* x, float* y) const
{
    *x = _positionR.x;
    *y = _positionR.y;
}

float ThunderSlot::getPositionX() const
{
    return _positionR.x;
}

Vec3 ThunderSlot::getPosition3D() const
{
    return Vec3(_positionR.x, _positionR.y, getPositionZ());
}

void ThunderSlot::setPositionX(float x)
{
    if (!_startingPositionInitialized) {
        _startingPositionInitialized = true;
    }
    _positionR.x = x;
}

float ThunderSlot::getPositionY() const
{
    return  _positionR.y;
}

void ThunderSlot::setPositionY(float y)
{
    if (!_startingPositionInitialized) {
        _startingPositionInitialized = true;
    }
    _positionR.y = y;
}

void ThunderSlot::tintWithColor(const Color3B& colors)
{
    setColor(colors);
    
    // Fast assignation
    for(unsigned int i = 0; i<_nuPoints*2; i++)
    {
        *((Color3B*) (_colorPointer+i*4)) = colors;
    }
}

Texture2D* ThunderSlot::getTexture(void) const
{
    return _texture;
}

void ThunderSlot::setTexture(Texture2D *texture)
{
    if (_texture != texture)
    {
        CC_SAFE_RETAIN(texture);
        CC_SAFE_RELEASE(_texture);
        _texture = texture;
        _texture->setTexParameters({GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT});
    }
}

void ThunderSlot::setBlendFunc(const BlendFunc &blendFunc)
{
    _blendFunc = blendFunc;
}

const BlendFunc& ThunderSlot::getBlendFunc(void) const
{
    return _blendFunc;
}

void ThunderSlot::setOpacity(GLubyte opacity)
{
    CCASSERT(false, "Set opacity no supported");
}

GLubyte ThunderSlot::getOpacity(void) const
{
    CCASSERT(false, "Opacity no supported");
    return 0;
}

void ThunderSlot::setOpacityModifyRGB(bool bValue)
{
    CC_UNUSED_PARAM(bValue);
}

bool ThunderSlot::isOpacityModifyRGB(void) const
{
    return false;
}

void ThunderSlot::update(float delta)
{
    if (!_startingPositionInitialized)
    {
        return;
    }
    static float curTime = 0;
    curTime += delta * 0.2f;
	std::stringstream str;
    _pointVertexes[0] = _positionR;
    Vec2 offset = Vec2(cosf(_angle), sinf(_angle)) * 20;
    for (int i=1; i<_maxPoints; ++i)
    {
        _pointVertexes[i] = _pointVertexes[i-1] + offset;
        _colorPointer[i*8+0] = 255;
        _colorPointer[i*8+1] = 255;
        _colorPointer[i*8+2] = 255;
        _colorPointer[i*8+3] = 255;
        _colorPointer[i*8+4] = 255;
        _colorPointer[i*8+5] = 255;
        _colorPointer[i*8+6] = 255;
        _colorPointer[i*8+7] = 255;

    }
    _nuPoints = _maxPoints;

    ccVertexLineToPolygon(_pointVertexes, _stroke, _vertices, 0, _nuPoints);
    
    float texDelta = 1.0f / _nuPoints;
    for(int i=0; i < _nuPoints; i++ ) {
        _texCoords[i*2] = Tex2F(0, (curTime + texDelta*i));
        _texCoords[i*2+1] = Tex2F(1, (curTime + texDelta*i));
    }
}

void ThunderSlot::reset()
{
    _nuPoints = 0;
}

void ThunderSlot::onDraw(const Mat4 &transform, uint32_t flags)
{
    getGLProgram()->use();
    getGLProgram()->setUniformsForBuiltins(transform);
    
    GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX );
    GL::blendFunc( _blendFunc.src, _blendFunc.dst );
    
    GL::bindTexture2D( _texture->getName() );
    
#ifdef EMSCRIPTEN
    // Size calculations from ::initWithFade
    setGLBufferData(_vertices, (sizeof(Vec2) * _maxPoints * 2), 0);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, 0);
    
    setGLBufferData(_texCoords, (sizeof(Tex2F) * _maxPoints * 2), 1);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, 0, 0);
    
    setGLBufferData(_colorPointer, (sizeof(GLubyte) * _maxPoints * 2 * 4), 2);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);
#else
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, _vertices);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, 0, _texCoords);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, _colorPointer);
#endif // EMSCRIPTEN
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)_nuPoints*2);
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, _nuPoints*2);
}

void ThunderSlot::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
    if(_nuPoints <= 1)
        return;
    _customCommand.init(_globalZOrder, transform, flags);
    _customCommand.func = CC_CALLBACK_0(ThunderSlot::onDraw, this, transform, flags);
    renderer->addCommand(&_customCommand);
}

