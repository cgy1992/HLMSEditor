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
#include "hlms_node_macroblock.h"
#include "hlms_node_porttypes.h"

//****************************************************************************/
HlmsNodeMacroblock::HlmsNodeMacroblock(QString title, QGraphicsItem* parent) :
    Magus::QtNode(title, parent),
    mMacroblockEnabled(true),
    mScissorTestEnabled(false),
    mDepthCheck(true),
    mDepthWrite(true),
    mDepthFunc(3),
    mDepthBiasConstant(0.0f),
    mDepthBiasSlopeScale(0.0f),
    mCullMode(1),
    mPolygonMode(2)
{
    // Define the connection policy
    HlmsPbsDatablockMacroblockPortType hlmsPbsDatablockMacroblockPortType;
    HlmsMacroblockDatablockPortType hlmsMacroblockDatablockPortType;
    hlmsPbsDatablockMacroblockPortType.addPortTypeToConnectionPolicy(hlmsMacroblockDatablockPortType);

    // Custome node settings
    setTitleColor(Qt::white);
    setHeaderTitleIcon(ICON_MACROBLOCK);
    setAction1Icon(ICON_MINMAX);
    setAction2Icon(ICON_CLOSE);
    alignTitle(Magus::ALIGNED_LEFT);
    setHeaderColor(QColor("#5D964E"));
    createPort(PORT_ID_MACROBLOCK,
               PORT_DATABLOCK,
               hlmsPbsDatablockMacroblockPortType,
               QColor("#5D964E"),
               Magus::PORT_SHAPE_CIRCLE,
               Magus::ALIGNED_LEFT,
               QColor("#5D964E"));

    setPortNameColor(Qt::white);
    setZoom(0.9);
}

//****************************************************************************/
HlmsNodeMacroblock::~HlmsNodeMacroblock(void)
{
}
