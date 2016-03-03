/****************************************************************************
**
** Copyright (C) 2016
**
** This file is generated by the Magus toolkit
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
****************************************************************************/

// Include
#include "constants.h"
#include "hlms_node_pbs_datablock.h"
#include "hlms_node_porttypes.h"

//****************************************************************************/
HlmsNodePbsDatablock::HlmsNodePbsDatablock(QString title, QGraphicsItem* parent) :
    QtNode(title, parent),
    mDiffuseRed(255.0f),
    mDiffuseGreen(255.0f),
    mDiffuseBlue(255.0f),
    mSpecularRed(255.0f),
    mSpecularGreen(255.0f),
    mSpecularBlue(255.0f),
    mRoughness(1.0f),
    mWorkflow(0),
    mMetalness(0.818f),
    mSeparateFresnel(false),
    mFresnelRed(0.818f),
    mFresnelGreen(0.818f),
    mFresnelBlue(0.818f),
    mTransparencyValue(1.0f),
    mTransparencyMode(0),
    mUseAlphaFromTexture(true),
    mBrdf(0),
    mAlphaTest(1),
    mAlphaTestThreshold(0.5f)
{
    mName = DEFAULT_PBS_DATABLOCK_NAME;

    // Define the connection policy
    HlmsPbsDatablockSamplerblockPortType hlmsPbsDatablockSamplerblockPortType;
    HlmsPbsDatablockMacroblockPortType hlmsPbsDatablockMacroblockPortType;
    HlmsPbsDatablockBlendblockPortType hlmsPbsDatablockBlendblockPortType;

    HlmsSamplerblockDatablockPortType hlmsSamplerblockDatablockPortType;
    HlmsBlendblockDatablockPortType hlmsBlendblockDatablockPortType;
    HlmsMacroblockDatablockPortType hlmsMacroblockDatablockPortType;
    hlmsSamplerblockDatablockPortType.addPortTypeToConnectionPolicy(hlmsPbsDatablockSamplerblockPortType);
    hlmsBlendblockDatablockPortType.addPortTypeToConnectionPolicy(hlmsPbsDatablockBlendblockPortType);
    hlmsMacroblockDatablockPortType.addPortTypeToConnectionPolicy(hlmsPbsDatablockMacroblockPortType);

    // Custom node settings
    setTitleColor(Qt::white);
    setHeaderTitleIcon(ICON_PBS_DATABLOCK);
    setAction1Icon(ICON_MINMAX);
    setAction2Icon(ICON_CLOSE);
    alignTitle(Magus::ALIGNED_LEFT);
    setHeaderColor(QColor("#5853BD"));

    // Sampler(s)
    createPort(PORT_ID_PBS_DATABLOCK,
               PORT_SAMPLERBLOCK,
               hlmsSamplerblockDatablockPortType,
               QColor("#874E96"),
               Magus::PORT_SHAPE_CIRCLE,
               Magus::ALIGNED_RIGHT,
               QColor("#874E96"));
    createPort(PORT_ID_PBS_DATABLOCK + 1,
               PORT_SAMPLERBLOCK,
               hlmsSamplerblockDatablockPortType,
               QColor("#874E96"),
               Magus::PORT_SHAPE_CIRCLE,
               Magus::ALIGNED_RIGHT,
               QColor("#874E96"));
    createPort(PORT_ID_PBS_DATABLOCK + 2,
               PORT_SAMPLERBLOCK,
               hlmsSamplerblockDatablockPortType,
               QColor("#874E96"),
               Magus::PORT_SHAPE_CIRCLE,
               Magus::ALIGNED_RIGHT,
               QColor("#874E96"));
    createPort(PORT_ID_PBS_DATABLOCK + 3,
               PORT_SAMPLERBLOCK,
               hlmsSamplerblockDatablockPortType,
               QColor("#874E96"),
               Magus::PORT_SHAPE_CIRCLE,
               Magus::ALIGNED_RIGHT,
               QColor("#874E96"));
    createPort(PORT_ID_PBS_DATABLOCK + 4,
               PORT_SAMPLERBLOCK,
               hlmsSamplerblockDatablockPortType,
               QColor("#874E96"),
               Magus::PORT_SHAPE_CIRCLE,
               Magus::ALIGNED_RIGHT,
               QColor("#874E96"));
    createPort(PORT_ID_PBS_DATABLOCK + 5,
               PORT_SAMPLERBLOCK,
               hlmsSamplerblockDatablockPortType,
               QColor("#874E96"),
               Magus::PORT_SHAPE_CIRCLE,
               Magus::ALIGNED_RIGHT,
               QColor("#874E96"));
    createPort(PORT_ID_PBS_DATABLOCK + 6,
               PORT_SAMPLERBLOCK,
               hlmsSamplerblockDatablockPortType,
               QColor("#874E96"),
               Magus::PORT_SHAPE_CIRCLE,
               Magus::ALIGNED_RIGHT,
               QColor("#874E96"));
    createPort(PORT_ID_PBS_DATABLOCK + 7,
               PORT_SAMPLERBLOCK,
               hlmsSamplerblockDatablockPortType,
               QColor("#874E96"),
               Magus::PORT_SHAPE_CIRCLE,
               Magus::ALIGNED_RIGHT,
               QColor("#874E96"));

    // Macro
    createPort(PORT_ID_PBS_DATABLOCK + 8,
               PORT_MACROBLOCK,
               hlmsMacroblockDatablockPortType,
               QColor("#5D964E"),
               Magus::PORT_SHAPE_CIRCLE,
               Magus::ALIGNED_RIGHT,
               QColor("#5D964E"));

    // Blend
    createPort(PORT_ID_PBS_DATABLOCK + 9,
               PORT_BLENDBLOCK,
               hlmsBlendblockDatablockPortType,
               QColor("#C74E4C"),
               Magus::PORT_SHAPE_CIRCLE,
               Magus::ALIGNED_RIGHT,
               QColor("#C74E4C"));

    setPortNameColor(Qt::white);
    setZoom(0.9);
}

//****************************************************************************/
HlmsNodePbsDatablock::~HlmsNodePbsDatablock(void)
{
}