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
#include "texture_tree.h"

//****************************************************************************/
TextureTreeDockWidget::TextureTreeDockWidget(const QString& iconDir, const QString& title, QMainWindow* parent, Qt::WindowFlags flags) :
    QDockWidget (title, parent, flags),
    mParent(parent)
{
    mIconDir = iconDir;
    mEmptyString = "";
    mTempString = "";

    // Context menu settings
    mResourceTreeWidget = new Magus::QtResourceTreeWidget(iconDir);
    mResourceTreeWidget->setCreateTopLevelGroupContextMenuItemEnabled(false);
    mResourceTreeWidget->setDeleteTopLevelGroupEnabled(false);
    mResourceTreeWidget->setTopLevelGroupItemEditable(false);
    mResourceTreeWidget->setCreateSubGroupContextMenuItemEnabled(true);
    mResourceTreeWidget->setCreateAssetContextMenuItemEnabled(false);
    mResourceTreeWidget->setDeleteResourceContextMenuItemEnabled(true);
    mResourceTreeWidget->setImportAssetContextMenuItemEnabled(false);
    mResourceTreeWidget->setDuplicateAssetContextMenuItemEnabled(false);

    // Toplevel group, sub group and assets settings
    mResourceTreeWidget->setSubLevelGroupItemEditable(true);
    mResourceTreeWidget->setAssetItemEditable(false);

    // Miscellanious settings
    mResourceTreeWidget->setSubgroupIconName(ICON_TEXTURE_SMALL_NO_PATH);
    mResourceTreeWidget->addCustomContextMenuItem(ACTION_IMPORT_TEXTURES_FROM_DIR);
    mResourceTreeWidget->addCustomContextMenuItem(ACTION_ADD_TEXTURES);

    // Listen to events.
    connect(mResourceTreeWidget, SIGNAL(resourceAdded(int)), this, SLOT(handleResourceAdded(int)));
    connect(mResourceTreeWidget, SIGNAL(resourceDeleted(int)), this, SLOT(handleResourceDeleted(int)));
    connect(mResourceTreeWidget, SIGNAL(resourceSelected(int)), this, SLOT(handleResourceSelected(int)));
    connect(mResourceTreeWidget, SIGNAL(resourceDoubleClicked(int)), this, SLOT(handleResourceDoubleClicked(int)));
    connect(mResourceTreeWidget, SIGNAL(resourceMoved(int)), this, SLOT(handleResourceMoved(int)));
    connect(mResourceTreeWidget, SIGNAL(resourceChanged(int)), this, SLOT(handleResourceChanged(int)));
    connect(mResourceTreeWidget, SIGNAL(resourceSearched(QString)), this, SLOT(handleResourceSearched(QString)));
    connect(mResourceTreeWidget, SIGNAL(resourceSearchReset()), this, SLOT(handleResourceSearchReset()));
    connect(mResourceTreeWidget, SIGNAL(customContextMenuItemSelected(QString)), this, SLOT(handleCustomContextMenuItemSelected(QString)));
    mInnerMain = new QMainWindow();
    mInnerMain->setCentralWidget(mResourceTreeWidget);
    setWidget(mInnerMain);
    QWidget* oldTitleBar = titleBarWidget();
    setTitleBarWidget(new QWidget());
    delete oldTitleBar;

    // Add toplevel group
    initializeResourceTree();
}

//****************************************************************************/
TextureTreeDockWidget::~TextureTreeDockWidget(void)
{
}

//****************************************************************************/
void TextureTreeDockWidget::initializeResourceTree (void)
{
    // Initialize; add toplevel groups to the mResourceTreeWidget up front
    QtSourcesInfo info;

    // Add to toplevel
    mResourceTreeWidget->addResource (TOOL_SOURCES_LEVEL_X000_TEXTURE,
                                      TOOL_SOURCES_LEVEL_X000_TEXTURE,
                                      0,
                                      QString("Textures"),
                                      QString("Textures"),
                                      ICON_TEXTURE_NO_PATH);

    info.toplevelId = TOOL_SOURCES_LEVEL_X000_TEXTURE;
    info.resourceId = TOOL_SOURCES_LEVEL_X000_TEXTURE;
    info.resourceType = Magus::TOOL_RESOURCETREE_KEY_TYPE_TOPLEVEL_GROUP;
    info.parentId = 0;
    info.fileName = QString("Textures");
    info.baseName = info.fileName;
    info.filter = QString("");
    info.baseNameThumb = ICON_TEXTURE_NO_PATH;
    mSourceInfo[TOOL_SOURCES_LEVEL_X000_TEXTURE] = info;

    // Determine what is enabled/disabled if a toplevelgroup is selected
    mResourceTreeWidget->enableContextMenuItemForTopLevelGroup(Magus::TOOL_RESOURCETREE_ACTION_CREATE_TOPLEVEL_GROUP, false);
    mResourceTreeWidget->enableContextMenuItemForTopLevelGroup(Magus::TOOL_RESOURCETREE_ACTION_CREATE_SUBGROUP, true);
    mResourceTreeWidget->enableContextMenuItemForTopLevelGroup(Magus::TOOL_RESOURCETREE_ACTION_CREATE_ASSET, false);
    mResourceTreeWidget->enableContextMenuItemForTopLevelGroup(Magus::TOOL_RESOURCETREE_ACTION_IMPORT_ASSET, false);
    mResourceTreeWidget->enableContextMenuItemForTopLevelGroup(Magus::TOOL_RESOURCETREE_ACTION_DUPLICATE_ASSET, false);
    mResourceTreeWidget->enableContextMenuItemForTopLevelGroup(Magus::TOOL_RESOURCETREE_ACTION_DELETE_RESOURCE, false);

    // Determine what is enabled/disabled if a subgroup is selected
    mResourceTreeWidget->enableContextMenuItemForSubGroup(Magus::TOOL_RESOURCETREE_ACTION_CREATE_TOPLEVEL_GROUP, false);
    mResourceTreeWidget->enableContextMenuItemForSubGroup(Magus::TOOL_RESOURCETREE_ACTION_CREATE_SUBGROUP, true);
    mResourceTreeWidget->enableContextMenuItemForSubGroup(Magus::TOOL_RESOURCETREE_ACTION_CREATE_ASSET, false);
    mResourceTreeWidget->enableContextMenuItemForSubGroup(Magus::TOOL_RESOURCETREE_ACTION_IMPORT_ASSET, false);
    mResourceTreeWidget->enableContextMenuItemForSubGroup(Magus::TOOL_RESOURCETREE_ACTION_DUPLICATE_ASSET, false);
    mResourceTreeWidget->enableContextMenuItemForSubGroup(Magus::TOOL_RESOURCETREE_ACTION_DELETE_RESOURCE, true);

    // Determine what is enabled/disabled if an asset is selected
    mResourceTreeWidget->enableContextMenuItemForAsset(Magus::TOOL_RESOURCETREE_ACTION_CREATE_TOPLEVEL_GROUP, false);
    mResourceTreeWidget->enableContextMenuItemForAsset(Magus::TOOL_RESOURCETREE_ACTION_CREATE_SUBGROUP, false);
    mResourceTreeWidget->enableContextMenuItemForAsset(Magus::TOOL_RESOURCETREE_ACTION_CREATE_ASSET, false);
    mResourceTreeWidget->enableContextMenuItemForAsset(Magus::TOOL_RESOURCETREE_ACTION_IMPORT_ASSET, false);
    mResourceTreeWidget->enableContextMenuItemForAsset(Magus::TOOL_RESOURCETREE_ACTION_DUPLICATE_ASSET, false);
    mResourceTreeWidget->enableContextMenuItemForAsset(Magus::TOOL_RESOURCETREE_ACTION_DELETE_RESOURCE, true);
}

//****************************************************************************/
const QString& TextureTreeDockWidget::setSelectAssetQuiet(const QString& fileName)
{
    int id = getResourceId(fileName);
    if (id != -1)
    {
        mResourceTreeWidget->selectResource(id, false);
        return mSourceInfo[id].fileName;
    }

    return mEmptyString;
}

//****************************************************************************/
int TextureTreeDockWidget::getResourceId(const QString& fileName)
{
    QMap<int, QtSourcesInfo>::iterator iter;
    QMap<int, QtSourcesInfo>::iterator iterStart = mSourceInfo.begin();
    QMap<int, QtSourcesInfo>::iterator iterEnd = mSourceInfo.end();
    for (iter = iterStart; iter != iterEnd; ++iter)
    {
        QtSourcesInfo info = iter.value();
        if (fileName == info.fileName)
            return info.resourceId;
    }

    return -1;
}

//****************************************************************************/
void TextureTreeDockWidget::addTextureFile (const QString& fileName)
{
    if (isResourceExisting(fileName))
        return;

    QString baseName = fileName;
    baseName = getBaseFileName(baseName);
    int resourceId = mResourceTreeWidget->addResource(TOOL_SOURCES_LEVEL_X000_TEXTURE,
                                                      baseName,
                                                      fileName,
                                                      QString(""),
                                                      true,
                                                      true);
    mResourceTreeWidget->expand(TOOL_SOURCES_LEVEL_X000_TEXTURE);

    // Also add/set it to mSourceInfo
    QtSourcesInfo info;
    info.toplevelId = TOOL_SOURCES_LEVEL_X000_TEXTURE;
    info.resourceId = resourceId;
    info.resourceType = Magus::TOOL_RESOURCETREE_KEY_TYPE_ASSET; // It is an asset
    info.parentId = TOOL_SOURCES_LEVEL_X000_TEXTURE;
    info.fileName = fileName;
    info.baseName = baseName;
    info.baseNameThumb = "";
    mSourceInfo[resourceId] = info;
}

//****************************************************************************/
void TextureTreeDockWidget::addTextureFile (const QString& fileName, int groupId)
{
    if (isResourceExisting(fileName))
        return;

    QString baseName = fileName;
    baseName = getBaseFileName(baseName);
    int resourceId = mResourceTreeWidget->addResource(groupId,
                                                      baseName,
                                                      fileName,
                                                      QString(""),
                                                      true,
                                                      true);
    mResourceTreeWidget->expand(groupId);

    // Also add/set it to mSourceInfo
    QtSourcesInfo info;
    info.toplevelId = TOOL_SOURCES_LEVEL_X000_TEXTURE;
    info.resourceId = resourceId;
    info.resourceType = Magus::TOOL_RESOURCETREE_KEY_TYPE_ASSET; // It is an asset
    info.parentId = groupId;
    info.fileName = fileName;
    info.baseName = baseName;
    info.baseNameThumb = "";
    mSourceInfo[resourceId] = info;
}

//****************************************************************************/
int TextureTreeDockWidget::addGroup (const QString& group)
{
    // Determine whether a new subgroup must be created
    bool createGroup = false;
    int groupId = mResourceTreeWidget->getResourceIdFromName(group);
    if (groupId < 0)
    {
        createGroup = true;
    }
    else
    {
        // The item is existing; is it a group (if not, create it)
        QTreeWidgetItem* item = mResourceTreeWidget->getResourceItem(groupId);
        if (!mResourceTreeWidget->isItemSubGroup(item))
        {
            createGroup = true;
        }
    }

    if (createGroup)
    {
        // A new group must be created
        groupId = mResourceTreeWidget->addResource(TOOL_SOURCES_LEVEL_X000_TEXTURE,
                                                   group,
                                                   group,
                                                   ICON_TEXTURE_SMALL_NO_PATH,
                                                   false,
                                                   true);

        // Also add/set it to mSourceInfo
        QtSourcesInfo info;
        info.toplevelId = TOOL_SOURCES_LEVEL_X000_TEXTURE;
        info.resourceId = groupId;
        info.resourceType = Magus::TOOL_RESOURCETREE_KEY_TYPE_GROUP; // It is a group
        info.parentId = TOOL_SOURCES_LEVEL_X000_TEXTURE;
        info.fileName = group;
        info.baseName = group;
        info.baseNameThumb = "";
        mSourceInfo[groupId] = info;
    }

    return groupId;
}

//****************************************************************************/
void TextureTreeDockWidget::deleteTextureFile(const QString& fileName)
{
    // First, delete it from mResourceTreeWidget
    int resourceId = mResourceTreeWidget->getResourceIdFromName(fileName);
    if (resourceId < 0)
        return;

    mResourceTreeWidget->deleteResource(resourceId);

    // find the underlying asset (filenames) + delete it from the mSourceInfo
    mSourceInfo.remove(resourceId);
}

//****************************************************************************/
void TextureTreeDockWidget::handleResourceSelected(int resourceId)
{
    // Determine which type is selected
    QMap<int, QtSourcesInfo>::iterator it = mSourceInfo.find(resourceId);
    if (it != mSourceInfo.end())
    {
        QtSourcesInfo info = it.value();
        emit resourceSelected(info.toplevelId, info.parentId, info.resourceId, info.fileName, info.baseNameThumb, info.resourceType);
    }
}

//****************************************************************************/
void TextureTreeDockWidget::handleResourceDoubleClicked(int resourceId)
{
    // Determine which type is selected
    QMap<int, QtSourcesInfo>::iterator it = mSourceInfo.find(resourceId);
    if (it != mSourceInfo.end())
    {
        QtSourcesInfo info = it.value();
        if (info.resourceType == Magus::TOOL_RESOURCETREE_KEY_TYPE_ASSET)
        {
            // Do not do something specific for a double click on a toplevel group or a (sub)group
            emit resourceDoubleClicked(info.toplevelId, info.parentId, info.resourceId, info.fileName, info.baseNameThumb);
        }
    }
}

//****************************************************************************/
void TextureTreeDockWidget::handleResourceSearched(const QString& searchPattern)
{
    // Signal
    emit resourceSearched(searchPattern);
}

//****************************************************************************/
void TextureTreeDockWidget::handleResourceSearchReset(void)
{
    // Signal
    emit resourceSearchReset();
}

//****************************************************************************/
void TextureTreeDockWidget::handleCustomContextMenuItemSelected(const QString& menuItemText)
{
    emit customContextMenuItemSelected(menuItemText);
}

//****************************************************************************/
void TextureTreeDockWidget::handleResourceAdded(int resourceId)
{
    // Is activated when a subgroup is created (created assets do not trigger this function); this subgroup must be added to mSourceInfo
    QtSourcesInfo info;
    int toplevelId = mResourceTreeWidget->getToplevelParentId(resourceId);
    QString name = mResourceTreeWidget->getResourceName(resourceId);
    QString fullQualifiedName = mResourceTreeWidget->getFullQualifiedName(resourceId);
    info.toplevelId = toplevelId;
    info.resourceId = resourceId;
    info.resourceType = mResourceTreeWidget->getTypeFromResourceId(resourceId);
    info.parentId = mResourceTreeWidget->getParentId(resourceId);
    info.fileName = fullQualifiedName;
    info.baseName = name;
    mSourceInfo[resourceId] = info;
    emit resourceAdded(info.toplevelId, info.parentId, info.resourceId, info.fileName, info.baseName, info.resourceType);
}

//****************************************************************************/
void TextureTreeDockWidget::handleResourceDeleted(int resourceId)
{
    // Do not handle resources with resourceId = 0
    if (resourceId == 0)
        return;

    // Get all the underlying assets (filenames) + delete them from the mSourceInfo
    foreach(QtSourcesInfo info, mSourceInfo)
    {
        if (info.parentId == resourceId)
            handleResourceDeleted(info.resourceId);
        else if (info.resourceId == resourceId)
        {
            // Delete from mSourceInfo
            emit resourceDeleted(info.toplevelId, info.parentId, info.resourceId, info.fileName, info.baseName);
            mSourceInfo.remove(resourceId);
        }
    }
}

//****************************************************************************/
void TextureTreeDockWidget::handleResourceMoved(int resourceId)
{
    // Search the info object
    QtSourcesInfo info;
    QMap<int, QtSourcesInfo>::iterator it = mSourceInfo.find(resourceId);
    if (it == mSourceInfo.end())
        return;

    // Update the parentId and toplevelId (Although in this version it is not possible to move to a different toplevel)
    // Emitting again is not needed, because nothing changed in the assets widget
    info = it.value();
    info.toplevelId = mResourceTreeWidget->getToplevelParentId(resourceId);
    info.parentId = mResourceTreeWidget->getParentId(resourceId);
    mSourceInfo[resourceId] = info; // Overwrite existing one
    emit resourceMoved(info.toplevelId, info.parentId, info.resourceId, info.fileName, info.baseName, info.resourceType);
}

//****************************************************************************/
void TextureTreeDockWidget::handleResourceChanged(int resourceId)
{
    // Search the info object
    QtSourcesInfo info;
    QMap<int, QtSourcesInfo>::iterator it = mSourceInfo.find(resourceId);
    if (it == mSourceInfo.end())
        return;

    info = it.value();
    int toplevelId = mResourceTreeWidget->getToplevelParentId(resourceId);
    QString name = mResourceTreeWidget->getResourceName(resourceId);
    QString fullQualifiedName = mResourceTreeWidget->getFullQualifiedName(resourceId);
    info.toplevelId = toplevelId;
    info.resourceId = resourceId;
    info.resourceType = mResourceTreeWidget->getTypeFromResourceId(resourceId);
    info.parentId = mResourceTreeWidget->getParentId(resourceId);
    info.fileName = fullQualifiedName;
    info.baseName = name;
    mSourceInfo[resourceId] = info; // Overwrite existing one
    emit resourceChanged(info.toplevelId, info.parentId, info.resourceId, info.fileName, info.baseName, info.resourceType);
}

//****************************************************************************/
void TextureTreeDockWidget::expand (int resourceId)
{
    mResourceTreeWidget->expand(resourceId);
}

//****************************************************************************/
bool TextureTreeDockWidget::isResourceExisting(const QString& fileName)
{
    QMap<int, QtSourcesInfo>::iterator iter;
    QMap<int, QtSourcesInfo>::iterator iterStart = mSourceInfo.begin();
    QMap<int, QtSourcesInfo>::iterator iterEnd = mSourceInfo.end();
    for (iter = iterStart; iter != iterEnd; ++iter)
    {
        QtSourcesInfo info = iter.value();
        if (fileName == info.fileName)
            return true;
    }

    return false;
}

//****************************************************************************/
QVector<Magus::QtResourceInfo*>& TextureTreeDockWidget::getResources (void)
{
    // Delegate to mResourceTreeWidget; this is the component that actually retrieves the data
    return mResourceTreeWidget->getResources();
}

//****************************************************************************/
QVector<Magus::QtResourceInfo*>& TextureTreeDockWidget::getResources (int resourceId)
{
    mFilteredResources.clear();
    QVector<Magus::QtResourceInfo*> resources = mResourceTreeWidget->getResources();
    foreach (Magus::QtResourceInfo* info, resources)
    {
        if (info->parentId == resourceId)
        {
            if (info->resourceType == Magus::TOOL_RESOURCETREE_KEY_TYPE_ASSET)
            {
                // Only append assets as part of this group.
                // No recursive searching
                mFilteredResources.append(info);
            }
        }
    }
    return mFilteredResources;
}

//****************************************************************************/
void TextureTreeDockWidget::setResources(const QVector<Magus::QtResourceInfo*>& resources)
{
    // Add resources to the resource tree
    mResourceTreeWidget->setResources(resources, true);

    // Iterate again through the list to make sure that all info is set
    QVectorIterator<Magus::QtResourceInfo*> it(resources);
    it.toFront();
    Magus::QtResourceInfo* resourceInfo;
    while (it.hasNext())
    {
        resourceInfo = it.next();
        QtSourcesInfo info;
        info.toplevelId = resourceInfo->topLevelId;
        info.resourceId = resourceInfo->resourceId;
        info.resourceType = resourceInfo->resourceType;
        info.parentId = resourceInfo->parentId;
        info.fileName = resourceInfo->fullQualifiedName;
        info.baseName = resourceInfo->resourceName;
        mSourceInfo[resourceInfo->resourceId] = info;
    }

    mResourceTreeWidget->expandAll();
}

//****************************************************************************/
const QString& TextureTreeDockWidget::getCurrentFileName (void)
{
    mTempString = QString("");
    int resourceId = mResourceTreeWidget->getSelectedResource();

    QMap<int, QtSourcesInfo>::iterator iter;
    QMap<int, QtSourcesInfo>::iterator iterStart = mSourceInfo.begin();
    QMap<int, QtSourcesInfo>::iterator iterEnd = mSourceInfo.end();
    for (iter = iterStart; iter != iterEnd; ++iter)
    {
        QtSourcesInfo info = iter.value();
        if (resourceId == info.resourceId && info.resourceType == Magus::TOOL_RESOURCETREE_KEY_TYPE_ASSET)
        {
            // Found it
            mTempString = info.fileName;
            return mTempString;
        }
    }

    return mTempString;
}

//****************************************************************************/
void TextureTreeDockWidget::clearContent(void)
{
    mResourceTreeWidget->clear();
    initializeResourceTree();
}