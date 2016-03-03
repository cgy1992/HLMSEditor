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
#include "asset_containerwidget.h"
#include "asset_propertywidget_select.h"
#include "asset_propertywidget_checkbox.h"
#include "hlms_properties_blenddblock.h"
#include "properties_dockwidget.h"
#include "hlms_node_blenddblock.h"

//****************************************************************************/
HlmsPropertiesBlendblock::HlmsPropertiesBlendblock(const QString& fileNameIcon, QWidget* parent) : QWidget(parent)
{
    mHlmsNodeBlendblock = 0;
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mAssetWidget = new Magus::QtAssetWidget(QString("Blendblock"), fileNameIcon, this);
    mAssetWidget->setFileNameIconCollapsed(ASSET_ICON_COLLAPSE);
    mAssetWidget->setFileNameIconExpanded(ASSET_ICON_EXPAND);
    mAssetWidget->setHeaderAction2Icon(ICON_INFO);
    mAssetWidget->setHeaderTitleBold();
    connect(mAssetWidget, SIGNAL(valueChanged(QtProperty*)), this, SLOT(propertyValueChanged(QtProperty*)));
    connect(mAssetWidget, SIGNAL(headerAction2Clicked()), this, SLOT(infoClicked(void)));

    // Create container - General
    Magus::QtContainerWidget* container = 0;
    container = mAssetWidget->createContainer(CONTAINER_BLENDBLOCK_GENERAL, QString(""));

    // Create properties
    // ******** Enabled ********
    mAssetWidget->createProperty(CONTAINER_BLENDBLOCK_GENERAL,
                                 PROPERTY_BLENDBLOCK_ENABLED,
                                 QString("Enabled"),
                                 Magus::QtProperty::CHECKBOX);

    // ******** Alpha To Coverage Enabled ********
    mAssetWidget->createProperty(CONTAINER_BLENDBLOCK_GENERAL,
                                 PROPERTY_BLENDBLOCK_ALPHA_TO_COVERAGE,
                                 QString("Alpha to coverage"),
                                 Magus::QtProperty::CHECKBOX);

    // ******** Blend Channel Mask ********
    QStringList stringListBlendChannelMask;
    stringListBlendChannelMask << QString("BlendChannelRed") <<
                                  QString("BlendChannelGreen") <<
                                  QString("BlendChannelBlue") <<
                                  QString("BlendChannelAlpha") <<
                                  QString("BlendChannelAll");
    Magus::QtSelectProperty* selectBlendChannelMaskProperty = static_cast<Magus::QtSelectProperty*>
            (mAssetWidget->createProperty(CONTAINER_BLENDBLOCK_GENERAL,
                                          PROPERTY_BLENDBLOCK_BLEND_CHANNEL_MASK,
                                          QString("Blend channel mask"),
                                          Magus::QtProperty::SELECT));
    selectBlendChannelMaskProperty->addValues(stringListBlendChannelMask);

    // ******** Transparent ********
    mAssetWidget->createProperty(CONTAINER_BLENDBLOCK_GENERAL,
                                 PROPERTY_BLENDBLOCK_TRANSPARENT,
                                 QString("Transparent"),
                                 Magus::QtProperty::CHECKBOX);

    // ******** Separate Blend ********
    mAssetWidget->createProperty(CONTAINER_BLENDBLOCK_GENERAL,
                                 PROPERTY_BLENDBLOCK_SEPARATE_BLEND,
                                 QString("Separate blend"),
                                 Magus::QtProperty::CHECKBOX);

    // ******** Source Blend Factor ********
    // SBF_ONE,
    // SBF_ZERO,
    // SBF_DEST_COLOUR,
    // SBF_SOURCE_COLOUR,
    // SBF_ONE_MINUS_DEST_COLOUR,
    // SBF_ONE_MINUS_SOURCE_COLOUR,
    // SBF_DEST_ALPHA,
    // SBF_SOURCE_ALPHA,
    // SBF_ONE_MINUS_DEST_ALPHA,
    // SBF_ONE_MINUS_SOURCE_ALPHA
    QStringList stringListSceneBlendFactor;
    stringListSceneBlendFactor << QString("One") <<
                                  QString("Zero") <<
                                  QString("Destination colour") <<
                                  QString("Source colour") <<
                                  QString("One minus dest. colour") <<
                                  QString("One minus source colour") <<
                                  QString("Destination alpha") <<
                                  QString("Source alpha") <<
                                  QString("One minus dest. alpha") <<
                                  QString("One minus source alpha");
    Magus::QtSelectProperty* selectSourceBlendFactorProperty = static_cast<Magus::QtSelectProperty*>
            (mAssetWidget->createProperty(CONTAINER_BLENDBLOCK_GENERAL,
                                          PROPERTY_BLENDBLOCK_SOURCE_BLEND_FACTOR,
                                          QString("Source blend factor"),
                                          Magus::QtProperty::SELECT));
    selectSourceBlendFactorProperty->addValues(stringListSceneBlendFactor);

    // ******** Dest Blend Factor ********
    Magus::QtSelectProperty* selectDestinationBlendFactorProperty = static_cast<Magus::QtSelectProperty*>
            (mAssetWidget->createProperty(CONTAINER_BLENDBLOCK_GENERAL,
                                          PROPERTY_BLENDBLOCK_DEST_BLEND_FACTOR,
                                          QString("Destination blend factor"),
                                          Magus::QtProperty::SELECT));
    selectDestinationBlendFactorProperty->addValues(stringListSceneBlendFactor);

    // ******** Source Blend Factor Alpha ********
    Magus::QtSelectProperty* selectSourceBlendFactorAlphaProperty = static_cast<Magus::QtSelectProperty*>
            (mAssetWidget->createProperty(CONTAINER_BLENDBLOCK_GENERAL,
                                          PROPERTY_BLENDBLOCK_SOURCE_BLEND_FACTOR_ALPHA,
                                          QString("Source blend factor alpha"),
                                          Magus::QtProperty::SELECT));
    selectSourceBlendFactorAlphaProperty->addValues(stringListSceneBlendFactor);

    // ******** Dest Blend Factor Alpha ********
    Magus::QtSelectProperty* selectDestinationBlendFactorAlphaProperty = static_cast<Magus::QtSelectProperty*>
            (mAssetWidget->createProperty(CONTAINER_BLENDBLOCK_GENERAL,
                                          PROPERTY_BLENDBLOCK_DEST_BLEND_FACTOR_ALPHA,
                                          QString("Dest. blend factor alpha"),
                                          Magus::QtProperty::SELECT));
    selectDestinationBlendFactorAlphaProperty->addValues(stringListSceneBlendFactor);

    // ******** Blend Operation ********
    // SBO_ADD,
    // SBO_SUBTRACT,
    // SBO_REVERSE_SUBTRACT,
    // SBO_MIN,
    // SBO_MAX
    QStringList stringListSceneBlendOperation;
    stringListSceneBlendOperation << QString("Add") <<
                                     QString("Subtract") <<
                                     QString("Reverse subtract") <<
                                     QString("Min") <<
                                     QString("Max");
    Magus::QtSelectProperty* selectBlendOperationProperty = static_cast<Magus::QtSelectProperty*>
            (mAssetWidget->createProperty(CONTAINER_BLENDBLOCK_GENERAL,
                                          PROPERTY_BLENDBLOCK_BLEND_OPERATION,
                                          QString("Blend operation"),
                                          Magus::QtProperty::SELECT));
    selectBlendOperationProperty->addValues(stringListSceneBlendOperation);

    // ******** Blend Operation Alpha ********
    Magus::QtSelectProperty* selectBlendOperationAlphaProperty = static_cast<Magus::QtSelectProperty*>
            (mAssetWidget->createProperty(CONTAINER_BLENDBLOCK_GENERAL,
                                          PROPERTY_BLENDBLOCK_BLEND_OPERATION_ALPHA,
                                          QString("Blend operation alpha"),
                                          Magus::QtProperty::SELECT));
    selectBlendOperationAlphaProperty->addValues(stringListSceneBlendOperation);

    // Layout
    mainLayout->addWidget(mAssetWidget);
    setLayout(mainLayout);
    mAssetWidget->expandAllContainers();
}

//****************************************************************************/
HlmsPropertiesBlendblock::~HlmsPropertiesBlendblock(void)
{
}

//****************************************************************************/
void HlmsPropertiesBlendblock::setObject (HlmsNodeBlendblock* hlmsNodeBlendblock)
{
    mHlmsNodeBlendblock = hlmsNodeBlendblock;
    Magus::QtSelectProperty* selectProperty;
    Magus::QtCheckBoxProperty* checkboxProperty;

    // ******** Enabled ********
    checkboxProperty = static_cast<Magus::QtCheckBoxProperty*>(mAssetWidget->getPropertyWidget(PROPERTY_BLENDBLOCK_ENABLED));
    checkboxProperty->setValue(hlmsNodeBlendblock->getBlendblockEnabled());

    // ******** Alpha To Coverage Enabled ********
    checkboxProperty = static_cast<Magus::QtCheckBoxProperty*>(mAssetWidget->getPropertyWidget(PROPERTY_BLENDBLOCK_ALPHA_TO_COVERAGE));
    checkboxProperty->setValue(hlmsNodeBlendblock->getAlphaToCoverageEnabled());

    // ******** Blend Channel Mask ********
    selectProperty = static_cast<Magus::QtSelectProperty*>(mAssetWidget->getPropertyWidget(PROPERTY_BLENDBLOCK_BLEND_CHANNEL_MASK));
    selectProperty->setCurentIndex(hlmsNodeBlendblock->getBlendChannelMask());

    // ******** Transparent ********
    checkboxProperty = static_cast<Magus::QtCheckBoxProperty*>(mAssetWidget->getPropertyWidget(PROPERTY_BLENDBLOCK_TRANSPARENT));
    checkboxProperty->setValue(hlmsNodeBlendblock->getTransparent());

    // ******** Separate Blend ********
    checkboxProperty = static_cast<Magus::QtCheckBoxProperty*>(mAssetWidget->getPropertyWidget(PROPERTY_BLENDBLOCK_SEPARATE_BLEND));
    checkboxProperty->setValue(hlmsNodeBlendblock->getSeparateBlend());

    // ******** Source Blend Factor ********
    selectProperty = static_cast<Magus::QtSelectProperty*>(mAssetWidget->getPropertyWidget(PROPERTY_BLENDBLOCK_SOURCE_BLEND_FACTOR));
    selectProperty->setCurentIndex(hlmsNodeBlendblock->getSourceBlendFactor());

    // ******** Dest Blend Factor ********
    selectProperty = static_cast<Magus::QtSelectProperty*>(mAssetWidget->getPropertyWidget(PROPERTY_BLENDBLOCK_DEST_BLEND_FACTOR));
    selectProperty->setCurentIndex(hlmsNodeBlendblock->getDestBlendFactor());

    // ******** Source Blend Factor Alpha ********
    selectProperty = static_cast<Magus::QtSelectProperty*>(mAssetWidget->getPropertyWidget(PROPERTY_BLENDBLOCK_SOURCE_BLEND_FACTOR_ALPHA));
    selectProperty->setCurentIndex(hlmsNodeBlendblock->getSourceBlendFactorAlpha());

    // ******** Dest Blend Factor Alpha ********
    selectProperty = static_cast<Magus::QtSelectProperty*>(mAssetWidget->getPropertyWidget(PROPERTY_BLENDBLOCK_DEST_BLEND_FACTOR_ALPHA));
    selectProperty->setCurentIndex(hlmsNodeBlendblock->getDestBlendFactorAlpha());

    // ******** Blend Operation ********
    selectProperty = static_cast<Magus::QtSelectProperty*>(mAssetWidget->getPropertyWidget(PROPERTY_BLENDBLOCK_BLEND_OPERATION));
    selectProperty->setCurentIndex(hlmsNodeBlendblock->getBlendOperation());

    // ******** Blend Operation Alpha ********
    selectProperty = static_cast<Magus::QtSelectProperty*>(mAssetWidget->getPropertyWidget(PROPERTY_BLENDBLOCK_BLEND_OPERATION_ALPHA));
    selectProperty->setCurentIndex(hlmsNodeBlendblock->getBlendOperationAlpha());
}

//****************************************************************************/
void HlmsPropertiesBlendblock::propertyValueChanged(QtProperty* property)
{
    if (!mHlmsNodeBlendblock || !property)
        return;

    Magus::QtSelectProperty* selectProperty;
    Magus::QtCheckBoxProperty* checkboxProperty;

    switch (property->mPropertyId)
    {
        case PROPERTY_BLENDBLOCK_ENABLED:
        {
            checkboxProperty = static_cast<Magus::QtCheckBoxProperty*>(property);
            mHlmsNodeBlendblock->setBlendblockEnabled(checkboxProperty->getValue());
        }
        break;
        case PROPERTY_BLENDBLOCK_ALPHA_TO_COVERAGE:
        {
            checkboxProperty = static_cast<Magus::QtCheckBoxProperty*>(property);
            mHlmsNodeBlendblock->setAlphaToCoverageEnabled(checkboxProperty->getValue());
        }
        break;
        case PROPERTY_BLENDBLOCK_BLEND_CHANNEL_MASK:
        {
            selectProperty = static_cast<Magus::QtSelectProperty*>(property);
            mHlmsNodeBlendblock->setBlendChannelMask(selectProperty->getCurrentIndex());
        }
        break;
        case PROPERTY_BLENDBLOCK_TRANSPARENT:
        {
            checkboxProperty = static_cast<Magus::QtCheckBoxProperty*>(property);
            mHlmsNodeBlendblock->setTransparent(checkboxProperty->getValue());
        }
        break;
        case PROPERTY_BLENDBLOCK_SEPARATE_BLEND:
        {
            checkboxProperty = static_cast<Magus::QtCheckBoxProperty*>(property);
            mHlmsNodeBlendblock->setSeparateBlend(checkboxProperty->getValue());
        }
        break;
        case PROPERTY_BLENDBLOCK_SOURCE_BLEND_FACTOR:
        {
            selectProperty = static_cast<Magus::QtSelectProperty*>(property);
            mHlmsNodeBlendblock->setSourceBlendFactor(selectProperty->getCurrentIndex());
        }
        break;
        case PROPERTY_BLENDBLOCK_DEST_BLEND_FACTOR:
        {
            selectProperty = static_cast<Magus::QtSelectProperty*>(property);
            mHlmsNodeBlendblock->setDestBlendFactor(selectProperty->getCurrentIndex());
        }
        break;
        case PROPERTY_BLENDBLOCK_SOURCE_BLEND_FACTOR_ALPHA:
        {
            selectProperty = static_cast<Magus::QtSelectProperty*>(property);
            mHlmsNodeBlendblock->setSourceBlendFactorAlpha(selectProperty->getCurrentIndex());
        }
        break;
        case PROPERTY_BLENDBLOCK_DEST_BLEND_FACTOR_ALPHA:
        {
            selectProperty = static_cast<Magus::QtSelectProperty*>(property);
            mHlmsNodeBlendblock->setDestBlendFactorAlpha(selectProperty->getCurrentIndex());
        }
        break;
        case PROPERTY_BLENDBLOCK_BLEND_OPERATION:
        {
            selectProperty = static_cast<Magus::QtSelectProperty*>(property);
            mHlmsNodeBlendblock->setBlendOperation(selectProperty->getCurrentIndex());
        }
        break;
        case PROPERTY_BLENDBLOCK_BLEND_OPERATION_ALPHA:
        {
            selectProperty = static_cast<Magus::QtSelectProperty*>(property);
            mHlmsNodeBlendblock->setBlendOperationAlpha(selectProperty->getCurrentIndex());
        }
        break;
    }
}

//****************************************************************************/
void HlmsPropertiesBlendblock::infoClicked(void)
{
    PropertiesDockWidget* parent = static_cast<PropertiesDockWidget*>(parentWidget());
    parent->displayInfo(INFO_BLENDBLOCK, QString("Info"));
}