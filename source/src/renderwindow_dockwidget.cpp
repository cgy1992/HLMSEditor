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
#include "mainwindow.h"
#include "OgreMeshManager2.h"
#include "renderwindow_dockwidget.h"
#include "paintlayer_manager.h"
#include "hlms_utils_manager.h"

//****************************************************************************/
UndoRedoStack::UndoRedoStack (void)
{
    clearStack();
}

//****************************************************************************/
UndoRedoStack::~UndoRedoStack (void)
{
}

//****************************************************************************/
void UndoRedoStack::clearStack (void)
{
    mStack.clear();
    mStackPointer = 0;
}

//****************************************************************************/
UndoRedoStack::UndoRedoStackEntry UndoRedoStack::undo (void)
{
    // Return the first entry if the pointer is at the start
    if (mStackPointer <= 0)
    {
        mStackPointer = 0;
        return mStack.at(0);
    }

    --mStackPointer;
    return mStack.at(mStackPointer);
}

//****************************************************************************/
UndoRedoStack::UndoRedoStackEntry UndoRedoStack::redo (void)
{
    // Return the last entry if the pointer is at the end
    if (mStackPointer >= mStack.size() - 1)
    {
        mStackPointer = mStack.size() - 1;
        return mStack.at(mStackPointer);
    }

    ++mStackPointer;
    return mStack.at(mStackPointer);
}

//****************************************************************************/
void UndoRedoStack::addEntry (const UndoRedoStackEntry& entry)
{
    mStack.append(entry);
    mStackPointer = mStack.size() - 1;
}


//****************************************************************************/
//****************************************************************************/
//****************************************************************************/
RenderwindowDockWidget::RenderwindowDockWidget(QString title, MainWindow* parent, Qt::WindowFlags flags) :
	QDockWidget (title, parent, flags), 
    mParent(parent),
    mButtonToggleModelAndLight(0),
    mButtonTogglePaint(0),
    mTogglePaintMode(false),
    mToggleOffsetTexture(false),
    mButtonMarker(0),
    mButtonToggleHoover(0),
    mButtonUndo(0),
    mButtonRedo(0),
    mButtonModelActive(true),
    mToggleHooverOn(false),
    mLightIcon(0),
    mButtonOffsetTexture(0),
    mOffsetTextureOnIcon(0),
    mOffsetTextureOffIcon(0),
    mPaintOnIcon(0),
    mPaintOffIcon(0),
    mModelIcon(0),
    mMarkerIcon(0),
    mHooverOnIcon(0),
    mHooverOffIcon(0),
    mPaintUndoOffIcon(0),
    mPaintUndoOnIcon(0),
    mPaintRedoOffIcon(0),
    mPaintRedoOnIcon(0),
    mCurrentPbsDatablockBlock(0),
    mCurrentUnlitDatablockBlock(0),
    mCurrentPbsTextureType (Ogre::PBSM_DIFFUSE),
    mCurrentUnlitTextureType(0)
{
    // Create a context menu
    installEventFilter(this);
    setContextMenuPolicy(Qt::CustomContextMenu);
    mContextMenu = new QMenu(this);
    mContextMenu->addAction(new QAction(ACTION_SET_CURRENT_MATERIAL, this));
    mMeshesSubMenu = mContextMenu->addMenu("Select mesh");
    mActionGroupMeshes = new QActionGroup(mMeshesSubMenu);
    mActionGroupMeshes->setExclusive(true);
    mContextMenu->addAction(new QAction(ACTION_TOGGLE_LIGHT_DIRECTION, this));
    mContextMenu->addAction(new QAction(ACTION_RESET_CAMERA, this));
    mContextMenu->addAction(new QAction(ACTION_TOGGLE_SUBMESH_SELECTION, this));
    mContextMenu->addAction(new QAction(ACTION_SELECT_BACKGROUND_COLOUR, this));

    // Add skyboxes to the context menu
    mSkyBoxSubMenu = mContextMenu->addMenu("Select skybox");
    mActionGroupSkyBox = new QActionGroup(mSkyBoxSubMenu);
    mActionGroupSkyBox->setExclusive(true);
    addSkyBoxNameToContextMenu (NO_SKYBOX, true);
    QSettings settings(FILE_SETTINGS, QSettings::IniFormat);
    unsigned int numberOfSkyboxes = settings.value(SETTINGS_NUMBER_OF_SKYBOXES).toInt();
    QString key;
    QString value;
    for (unsigned int i = 0; i < numberOfSkyboxes; ++i)
    {
        key = SETTINGS_PREFIX_SKYBOX + QVariant(i).toString();
        value = settings.value(key).toString();
        if (!value.isEmpty())
        {
            addSkyBoxNameToContextMenu (value, false);
        }
    }

    // Define signal and slot for the context menu
    connect(mContextMenu, SIGNAL(triggered(QAction*)), this, SLOT(contextMenuSelected(QAction*)));

    // Create the meshMap
    QFile file(QString("models.cfg"));
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream readFile(&file);
    MeshStruct meshStruct;
    QString line;
    key = "";
    QString x;
    QString y;
    QString z;
    bool first = true;
    while (!readFile.atEnd())
    {
        line = readFile.readLine();
        QStringList elements = line.split(' ', QString::SkipEmptyParts);
        if (elements.size() == 5)
        {
            // Only 5 items
            key = elements[0];
            meshStruct.meshName = elements[1];
            x = elements[2];
            y = elements[3];
            z = elements[4];
            QVector3D scale(x.toFloat(), y.toFloat(), z.toFloat());
            meshStruct.scale = scale;
            mMeshMap.insert(key, meshStruct);

            // Also add it to the context menu
            if (first)
            {
                addMeshNameToContextMenu(key, true);
                first = false;
            }
            else
                addMeshNameToContextMenu(key, false);
        }
    }

    mInnerMain = new QMainWindow();
    mInnerMain->setMinimumSize(100,100);
    setWidget(mInnerMain);

    // Perform standard functions
    createActions();
    createMenus();
    createToolBars();

    mOgreWidget = new Magus::QOgreWidget();
    mOgreWidget->setRenderwindowDockWidget(this);
    mInnerMain->setCentralWidget(mOgreWidget);
    parent->getOgreManager()->registerOgreWidget(OGRE_WIDGET_RENDERWINDOW, mOgreWidget);
    mOgreWidget->createRenderWindow(parent->getOgreManager());

    // Set the max. width of the renderwindow
    QRect rec = QApplication::desktop()->screenGeometry();
    setMaximumWidth(1.78 * rec.height()); // 1.78 = 16/9

    // Load the list with meshes
    preLoadMeshMap();

    mOgreWidget->setFocus();
}

//****************************************************************************/
RenderwindowDockWidget::~RenderwindowDockWidget(void)
{
}

//****************************************************************************/
void RenderwindowDockWidget::mousePressEventPublic( QMouseEvent* e )
{
    mousePressEvent(e);
}

//****************************************************************************/
void RenderwindowDockWidget::enterEventPublic (QEvent* event)
{
    /* If painting is enabled and the mouse cursor is default, it must be set to non-default (cross)
     * as soon as the mouse enters the window.
     * Because the RenderwindowDockWidget doesn't get the events itself, it has to rely on the Ogre widget.
     */
    if (mTogglePaintMode)
    {
        if (QApplication::overrideCursor() && QApplication::overrideCursor()->shape() == Qt::CrossCursor)
            return;

        QApplication::setOverrideCursor(Qt::CrossCursor);
    }
    else if (mToggleOffsetTexture)
    {
        if (QApplication::overrideCursor() && QApplication::overrideCursor()->shape() == Qt::OpenHandCursor)
            return;

        QApplication::setOverrideCursor(Qt::OpenHandCursor);
    }
}

//****************************************************************************/
void RenderwindowDockWidget::leaveEventPublic (QEvent* event)
{
    /* If painting is enabled and the mouse cursor is non-default (cross), it must be set to default
     * as soon as the mouse leaves the window.
     * Because the RenderwindowDockWidget doesn't get the events itself, it has to rely on the Ogre widget.
     */
    if (mTogglePaintMode)
    {
        if (QApplication::overrideCursor() && QApplication::overrideCursor()->shape() == Qt::CrossCursor)
            QApplication::restoreOverrideCursor();
    }
    else if (mToggleOffsetTexture)
    {
        if (QApplication::overrideCursor() && QApplication::overrideCursor()->shape() == Qt::OpenHandCursor)
            QApplication::restoreOverrideCursor();
    }
}

//****************************************************************************/
void RenderwindowDockWidget::addUndoRedoStackEntry (Ogre::PbsTextureTypes textureType, Ogre::ushort textureSequence)
{
    UndoRedoStack::UndoRedoStackEntry entry;
    entry.textureType = textureType;
    entry.textureSequence = textureSequence;
    mUndoRedoStack.addEntry(entry);
}

//****************************************************************************/
void RenderwindowDockWidget::clearUndoRedoStackEntry (void)
{
    mUndoRedoStack.clearStack();
}

//****************************************************************************/
void RenderwindowDockWidget::mousePressEvent( QMouseEvent* e )
{
    if (e->button() == Qt::RightButton)
    {
        QPoint pos;
        pos.setX(e->screenPos().x());
        pos.setY(e->screenPos().y());
        mContextMenu->popup(pos);
    }
}

//-------------------------------------------------------------------------------------
void RenderwindowDockWidget::resizeEvent(QResizeEvent *e)
{
    // The heigth of the renderwindow follows the width, because the renderwindow must alway be
    // square; this is because the render-texture for picking and highlighting is square
    // Note, that this setup makes manual resizing of the height impossible
    if(e->isAccepted())
    {
        setMinimumHeight(0.5625 * e->size().width()); // 0.5625 = 9/16
        setMaximumHeight(0.5625 * e->size().width());
    }
}

//****************************************************************************/
void RenderwindowDockWidget::addToMeshMap(const QString name,
                                          const QString meshName,
                                          QVector3D scale)
{
    MeshStruct meshStruct;
    meshStruct.meshName = meshName;
    meshStruct.scale = scale;
    mMeshMap.insert(name, meshStruct);
    QAction* mesh = new QAction(name, this);
    mMeshMenu->addAction(mesh);
    addMeshNameToContextMenu(name, true);
}

//****************************************************************************/
void RenderwindowDockWidget::preLoadMeshMap(void)
{
    Ogre::String meshName;
    QMap<QString, MeshStruct>::iterator it;
    QMap<QString, MeshStruct>::iterator itStart = mMeshMap.begin();
    QMap<QString, MeshStruct>::iterator itEnd = mMeshMap.end();
    for (it = itStart; it != itEnd; ++it)
    {
        meshName = it.value().meshName.toStdString();
        Ogre::MeshPtr v2MeshPtr = Ogre::MeshManager::getSingleton().load(
                    meshName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    }
}

//****************************************************************************/
void RenderwindowDockWidget::createActions(void)
{
    mChangeBackgroundAction = new QAction(QIcon(ICON_COLOUR), QString("Change background colour"), this);
    connect(mChangeBackgroundAction, SIGNAL(triggered()), this, SLOT(doChangeBackgroundAction()));
}

//****************************************************************************/
void RenderwindowDockWidget::createMenus(void)
{
    QAction* mesh;
    QMenuBar* menuBar = mInnerMain->menuBar();
    mMeshMenu = menuBar->addMenu(QString("Change mesh"));

    QMap<QString, MeshStruct>::iterator it;
    QMap<QString, MeshStruct>::iterator itStart = mMeshMap.begin();
    QMap<QString, MeshStruct>::iterator itEnd = mMeshMap.end();
    for (it = itStart; it != itEnd; ++it)
    {
        mesh = new QAction(it.key(), this);
        mMeshMenu->addAction(mesh);
    }
    connect(mMeshMenu, SIGNAL(triggered(QAction*)), this, SLOT(doChangeMeshAction(QAction*)));
}

//****************************************************************************/
void RenderwindowDockWidget::createToolBars(void)
{
    // Toolbar
    mHToolBar = new QToolBar();

    // Button to switch between model and light movement/rotation
    mButtonToggleModelAndLight = new QPushButton();
    mButtonTogglePaint = new QPushButton();
    mButtonMarker = new QPushButton();
    mButtonToggleHoover = new QPushButton();
    mButtonOffsetTexture = new QPushButton();
    mButtonUndo = new QPushButton();
    mButtonRedo = new QPushButton();

    mModelIcon = new QIcon(ICON_MODEL);
    mLightIcon = new QIcon(ICON_LIGHT);
    mOffsetTextureOnIcon = new QIcon(ICON_MOVE_TEXTURE_ON);
    mOffsetTextureOffIcon = new QIcon(ICON_MOVE_TEXTURE_OFF);
    mPaintOnIcon = new QIcon(ICON_PAINT_ON);
    mPaintOffIcon = new QIcon(ICON_PAINT_OFF);
    mMarkerIcon = new QIcon(ICON_MARKER);
    mHooverOnIcon = new QIcon(ICON_HOOVER_ON);
    mHooverOffIcon = new QIcon(ICON_HOOVER_OFF);
    mOffsetTextureOnIcon = new QIcon(ICON_MOVE_TEXTURE_ON);
    mOffsetTextureOffIcon = new QIcon(ICON_MOVE_TEXTURE_OFF);
    mPaintUndoOffIcon = new QIcon(ICON_UNDO_OFF);
    mPaintUndoOnIcon = new QIcon(ICON_UNDO_ON);
    mPaintRedoOffIcon = new QIcon(ICON_REDO_OFF);
    mPaintRedoOnIcon = new QIcon(ICON_REDO_ON);

    mButtonToggleModelAndLight->setIcon(*mModelIcon);
    mButtonToggleModelAndLight->setToolTip(ACTION_TOGGLE_LIGHT_DIRECTION);
    mButtonTogglePaint->setIcon(*mPaintOffIcon);
    mButtonTogglePaint->setToolTip(ACTION_TOGGLE_PAINT);
    mButtonMarker->setIcon(*mMarkerIcon);
    mButtonMarker->setToolTip(ACTION_RESET_CAMERA);
    mButtonToggleHoover->setIcon(*mHooverOffIcon);
    mButtonToggleHoover->setToolTip(ACTION_TOGGLE_SUBMESH_SELECTION);
    mButtonOffsetTexture->setIcon(*mOffsetTextureOffIcon);
    mButtonOffsetTexture->setToolTip(ACTION_MOVE_TEXTURE);
    mButtonUndo->setIcon(*mPaintUndoOffIcon);
    mButtonUndo->setToolTip(ACTION_UNDO_PAINT);
    mButtonRedo->setIcon(*mPaintRedoOffIcon);
    mButtonRedo->setToolTip(ACTION_REDO_PAINT);

    connect(mButtonToggleModelAndLight, SIGNAL(clicked(bool)), this, SLOT(handleToggleModelAndLight()));
    connect(mButtonTogglePaint, SIGNAL(clicked(bool)), this, SLOT(handleTogglePaintMode()));
    connect(mButtonMarker, SIGNAL(clicked(bool)), this, SLOT(handleMarker()));
    connect(mButtonToggleHoover, SIGNAL(clicked(bool)), this, SLOT(handleToggleHoover()));
    connect(mButtonOffsetTexture, SIGNAL(clicked(bool)), this, SLOT(handleToggleOffsetTexture()));
    connect(mButtonUndo, SIGNAL(clicked(bool)), this, SLOT(handleUndo()));
    connect(mButtonRedo, SIGNAL(clicked(bool)), this, SLOT(handleRedo()));

    // Transformation widget
    mTransformationWidget = new Magus::TransformationWidget(mHToolBar);
    mTransformationWidget->setMaximumWidth(344);
    mTransformationWidget->setCurrentIndex(2); // Only scale is visible
    mTransformationWidget->setListEnabled(false);
    mInnerMain->addToolBar(Qt::TopToolBarArea, mHToolBar);
    mHToolBar->setMinimumHeight(32);
    //mHToolBar->setMinimumWidth(8 * 32);
    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Add widgets
    mHToolBar->addWidget(mTransformationWidget);
    mHToolBar->addWidget(mButtonToggleModelAndLight);
    mHToolBar->addWidget(mButtonMarker);
    mHToolBar->addWidget(mButtonToggleHoover);
    mHToolBar->addWidget(mButtonOffsetTexture);
    mHToolBar->addSeparator();
    mHToolBar->addWidget(mButtonTogglePaint);
    mHToolBar->addWidget(mButtonUndo);
    mHToolBar->addWidget(mButtonRedo);
    mHToolBar->addSeparator();
    mHToolBar->addWidget(spacer);
    mHToolBar->addAction(mChangeBackgroundAction);
    connect(mTransformationWidget, SIGNAL(valueChanged()), this, SLOT(doTransformationWidgetValueChanged()));
}

//****************************************************************************/
void RenderwindowDockWidget::doChangeMeshAction(QAction* action)
{
    QMap<QString, MeshStruct>::iterator it;
    QMap<QString, MeshStruct>::iterator itStart = mMeshMap.begin();
    QMap<QString, MeshStruct>::iterator itEnd = mMeshMap.end();
    MeshStruct meshStruct;
    for (it = itStart; it != itEnd; ++it)
    {
        QString actionText = action->text();
        if (action->text() == it.key())
        {
            meshStruct = it.value();
            Ogre::Vector3 scale(meshStruct.scale.x(),
                                meshStruct.scale.y(),
                                meshStruct.scale.z());
            Ogre::String meshName = meshStruct.meshName.toStdString();
            mOgreWidget->createItem(meshName, scale);
            updateTransformationWidgetFromOgreWidget();
            setCheckedMeshNameInContextMenu(it.key()); // Point to the selected meshname in the contextmenu
        }
    }
}

//****************************************************************************/
void RenderwindowDockWidget::handleToggleModelAndLight(void)
{
    setModelAndLight(!mButtonModelActive);
}

//****************************************************************************/
void RenderwindowDockWidget::setModelAndLight(bool enabled)
{
    mButtonModelActive = enabled;
    if (mButtonModelActive)
    {
        // Disable Light axis
        mButtonToggleModelAndLight->setIcon(*mModelIcon);
        mOgreWidget->enableLightItem(false);
    }
    else
    {
        // Enable Light axis; painting and move texture are off
        mButtonToggleModelAndLight->setIcon(*mLightIcon);
        mOgreWidget->enableLightItem(true);
        setPaintMode(false);
        setOffsetTextureMode(false);
    }
    mOgreWidget->setFocus();
}

//****************************************************************************/
void RenderwindowDockWidget::handleTogglePaintMode(void)
{
    setPaintMode(!mTogglePaintMode);
}

//****************************************************************************/
void RenderwindowDockWidget::setPaintMode(bool enabled)
{
    mTogglePaintMode = enabled;
    if (mTogglePaintMode)
    {
        // Painting is on, light axis is disabled and hoover is off
        mButtonTogglePaint->setIcon(*mPaintOnIcon);
        mButtonUndo->setIcon(*mPaintUndoOnIcon);
        mButtonRedo->setIcon(*mPaintRedoOnIcon);
        setModelAndLight(true); // Show model, not the light
        setHoover(false);
        setOffsetTextureMode(false);
    }
    else
    {
        QApplication::restoreOverrideCursor();
        mButtonTogglePaint->setIcon(*mPaintOffIcon);
        mButtonUndo->setIcon(*mPaintUndoOffIcon);
        mButtonRedo->setIcon(*mPaintRedoOffIcon);
    }
    mOgreWidget->setFocus();
    mOgreWidget->setPaintMode(mTogglePaintMode);
}

//****************************************************************************/
void RenderwindowDockWidget::handleMarker(void)
{
    mOgreWidget->resetCamera();
}


//****************************************************************************/
void RenderwindowDockWidget::setHoover(bool enabled)
{
    mToggleHooverOn = enabled;
    if (mToggleHooverOn)
    {
        // If hoover is on, painting and move texture are off
        mButtonToggleHoover->setIcon(*mHooverOnIcon);
        setPaintMode(false);
        setOffsetTextureMode(false);
    }
    else
    {
        mButtonToggleHoover->setIcon(*mHooverOffIcon);
    }
    mOgreWidget->setHoover(mToggleHooverOn);
    mOgreWidget->setFocus();
}

//****************************************************************************/
void RenderwindowDockWidget::handleToggleHoover(void)
{
    setHoover(!mToggleHooverOn);
}

//****************************************************************************/
void RenderwindowDockWidget::handleToggleOffsetTexture(void)
{
    setOffsetTextureMode (!mToggleOffsetTexture, true);
}

//****************************************************************************/
void RenderwindowDockWidget::setOffsetTextureMode(bool enabled, bool showMessage)
{
    if (mCurrentPbsDatablockBlock || mCurrentUnlitDatablockBlock)
    {
        mToggleOffsetTexture = enabled;
        if (enabled)
        {
            // Activate the 'move texture' function; other options are off
            mButtonOffsetTexture->setIcon(*mOffsetTextureOnIcon);
            mOgreWidget->setOffsetTextureMode(true,
                                              mCurrentPbsDatablockBlock,
                                              mCurrentUnlitDatablockBlock,
                                              mCurrentPbsTextureType,
                                              mCurrentUnlitTextureType);
            setModelAndLight(true); // Show model, not the light
            setHoover(false);
            setPaintMode(false);
        }
        else
        {
            // Deactivate the 'move texture' function
            QApplication::restoreOverrideCursor();
            mButtonOffsetTexture->setIcon(*mOffsetTextureOffIcon);
            mOgreWidget->setOffsetTextureMode(false);
        }
    }
    else
    {
        if (showMessage)
            QMessageBox::information(0, QString("Warning"), QString("Please select a samplerblock first. It must match the following conditions:\n"
                                                                    "- In case of PBS: Only detal diffuse textures and detail normal textures are allowed.\n"
                                                                    "- In case of Unlit: All texture types are allowed."));
    }
}

//****************************************************************************/
void RenderwindowDockWidget::handleUndo(void)
{
    if (mTogglePaintMode)
    {
        UndoRedoStack::UndoRedoStackEntry entry = mUndoRedoStack.undo();
        mParent->loadTextureGeneration (entry.textureType, entry.textureSequence);
    }
}

//****************************************************************************/
void RenderwindowDockWidget::handleRedo(void)
{
    if (mTogglePaintMode)
    {
        UndoRedoStack::UndoRedoStackEntry entry = mUndoRedoStack.redo();
        mParent->loadTextureGeneration (entry.textureType, entry.textureSequence);
    }
}

//****************************************************************************/
void RenderwindowDockWidget::updateTransformationWidgetFromOgreWidget(void)
{
    if (!mOgreWidget)
        return;

    QVector3D v;

    // Scale
    v.setX(mOgreWidget->getItemScale().x);
    v.setY(mOgreWidget->getItemScale().y);
    v.setZ(mOgreWidget->getItemScale().z);
    mTransformationWidget->setScale(v);
}

//****************************************************************************/
void RenderwindowDockWidget::doTransformationWidgetValueChanged(void)
{
    if (!mOgreWidget)
        return;

    // Replace the code in this function with your own code.
    switch (mTransformationWidget->getCurrentTransformation())
    {
        case Magus::TransformationWidget::SCALE:
        {
            Ogre::Vector3 v;
            v.x = mTransformationWidget->getScale().x();
            v.y = mTransformationWidget->getScale().y();
            v.z = mTransformationWidget->getScale().z();
            mOgreWidget->setItemScale(v);
        }
        break;
    }
}

//****************************************************************************/
void RenderwindowDockWidget::doChangeBackgroundAction(void)
{
    QColorDialog dialog;
    QColor c = dialog.getColor();
    Ogre::ColourValue colour(c.red()/255.0f, c.green()/255.0f, c.blue()/255.0f, 1.0f);
    mOgreWidget->setBackgroundColour(colour);
}

//****************************************************************************/
void RenderwindowDockWidget::addMeshNameToContextMenu (const QString& meshName, bool checked)
{
    // First set all items to unchecked (only if checked is 'true')
    QAction* action;
    if (checked)
    {
        foreach (action, mActionGroupMeshes->actions())
            action->setChecked(false);
    }

    action = new QAction(meshName, this);
    action->setCheckable(true);
    action->setChecked(checked); // Set the added one to checked/unchecked
    mActionGroupMeshes->addAction(action);
    mMeshesSubMenu->addActions(mActionGroupMeshes->actions());
}

//****************************************************************************/
void RenderwindowDockWidget::setCheckedMeshNameInContextMenu (const QString& meshName)
{
    QAction* action;
    foreach (action, mActionGroupMeshes->actions())
    {
        if (action->text() == meshName)
            action->setChecked(true);
        else
            action->setChecked(false);
    }
}

//****************************************************************************/
void RenderwindowDockWidget::addSkyBoxNameToContextMenu (const QString& skyBoxName, bool checked)
{
    // First set all items to unchecked (only if checked is 'true')
    QAction* action;
    if (checked)
    {
        foreach (action, mActionGroupSkyBox->actions())
            action->setChecked(false);
    }

    action = new QAction(skyBoxName, this);
    action->setCheckable(true);
    action->setChecked(checked); // Set the added one to checked/unchecked
    mActionGroupSkyBox->addAction(action);
    mSkyBoxSubMenu->addActions(mActionGroupSkyBox->actions());
}

//****************************************************************************/
void RenderwindowDockWidget::setCheckedSkyBoxNameInContextMenu (const QString& skyBoxName)
{
    QAction* action;
    foreach (action, mActionGroupSkyBox->actions())
    {
        if (action->text() == skyBoxName)
            action->setChecked(true);
        else
            action->setChecked(false);
    }
}

//****************************************************************************/
void RenderwindowDockWidget::contextMenuSelected(QAction* action)
{
    if (action->text() == ACTION_SET_CURRENT_MATERIAL)
    {
        mOgreWidget->assignCurrentDatablock();
        return;
    }
    else if (action->text() == ACTION_TOGGLE_LIGHT_DIRECTION)
    {
        handleToggleModelAndLight();
        return;
    }
    else if (action->text() == ACTION_RESET_CAMERA)
    {
        handleMarker();
        return;
    }
    else if (action->text() == ACTION_TOGGLE_SUBMESH_SELECTION)
    {
        handleToggleHoover();
        return;
    }
    else if (action->text() == ACTION_SELECT_BACKGROUND_COLOUR)
    {
        doChangeBackgroundAction();
        return;
    }
    else
    {
        // It could be a skybox
        if (action->text() == NO_SKYBOX)
        {
            // Make skybox node invisible
            mOgreWidget->setSkyBoxVisible(false);
            return;
        }
        else
        {
            QSettings settings(FILE_SETTINGS, QSettings::IniFormat);
            unsigned int numberOfSkyboxes = settings.value(SETTINGS_NUMBER_OF_SKYBOXES).toInt();
            QString key;
            QString value;
            for (unsigned int i = 0; i < numberOfSkyboxes; ++i)
            {
                key = SETTINGS_PREFIX_SKYBOX + QVariant(i).toString();
                value = settings.value(key).toString();
                if (value == action->text())
                {
                    // A skybox was selected; create it
                    mOgreWidget->updateSkyBoxMaterial(value.toStdString());
                    return;
                }
            }
        }
    }

    // Action is not recognized; it is probably a selected mesh
    doChangeMeshAction(action);
}

//****************************************************************************/
void RenderwindowDockWidget::notifyHlmsPropertiesPbsSamplerblockVisible(bool visible,
                                                                        const Ogre::IdString& datablockId,
                                                                        const Ogre::PbsTextureTypes textureType)
{
    if (visible && (textureType == Ogre::PBSM_DETAIL0 ||
                    textureType == Ogre::PBSM_DETAIL1 ||
                    textureType == Ogre::PBSM_DETAIL2 ||
                    textureType == Ogre::PBSM_DETAIL3 ||
                    textureType == Ogre::PBSM_DETAIL0_NM ||
                    textureType == Ogre::PBSM_DETAIL1_NM ||
                    textureType == Ogre::PBSM_DETAIL2_NM ||
                    textureType == Ogre::PBSM_DETAIL3_NM))
    {
        // It is allowed to set the icon to 'on' (it can only be set to 'on' if the user clicks on it)
        HlmsUtilsManager* hlmsUtilManager = mParent->getHlmsUtilsManager(); // Use the HlmsUtilsManager of the mainwindow
        mCurrentPbsDatablockBlock = hlmsUtilManager->getPbsDatablock(datablockId);
        mCurrentPbsTextureType = textureType;
    }
    else
    {
        // Deactivate moving texture
        mButtonOffsetTexture->setIcon(*mOffsetTextureOffIcon);
        mOgreWidget->setOffsetTextureMode(false);
        mCurrentPbsDatablockBlock = 0;
        mCurrentPbsTextureType = Ogre::PBSM_DIFFUSE;
        mToggleOffsetTexture = false;
    }
}

//****************************************************************************/
void RenderwindowDockWidget::notifyHlmsPropertiesUnlitSamplerblockVisible(bool visible,
                                                                          const Ogre::IdString& datablockId,
                                                                          Ogre::uint8 textureType)
{
    if (visible)
    {
        // It is allowed to set the icon to 'on' (it can only be set to 'on' if the user clicks on it)
        HlmsUtilsManager* hlmsUtilManager = mParent->getHlmsUtilsManager(); // Use the HlmsUtilsManager of the mainwindow
        mCurrentUnlitDatablockBlock = hlmsUtilManager->getUnlitDatablock(datablockId);
        mCurrentUnlitTextureType = textureType;
    }
    else
    {
        // Deactivate moving texture
        mButtonOffsetTexture->setIcon(*mOffsetTextureOffIcon);
        mOgreWidget->setOffsetTextureMode(false);
        mCurrentUnlitDatablockBlock = 0;
        mCurrentUnlitTextureType = 0;
    }
}

//****************************************************************************/
void RenderwindowDockWidget::notifyOffsetTextureUpdated (float offsetX, float offsetY)
{
    // The offset was changed by means of the Ogre widget, so the properties widget must be updated
    HlmsPropertiesSamplerblock* samplerProperties = mParent->mPropertiesDockWidget->mHlmsPropertiesSamplerblock;
    if (samplerProperties)
    {
        HlmsNodeSamplerblock* samplerBlock = samplerProperties->getHlmsNodeSamplerblock ();
        if (samplerBlock)
        {
            QVector2D v2;
            v2.setX(offsetX);
            v2.setY(offsetY);
            if (mCurrentPbsDatablockBlock)
            {
                samplerBlock->setOffset(v2);
            }
            else if (mCurrentUnlitDatablockBlock)
            {
                samplerBlock->setAnimationTranslate(v2);
                samplerBlock->setAnimationEnabled(true);
            }
            samplerProperties->updateOffsetPropertiesExternal(samplerBlock);
        }
    }
}
