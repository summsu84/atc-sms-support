#include "WINDOW.h"
#include "RenderArea.h"

const int IdRole = Qt::UserRole;

WINDOW::WINDOW(QWidget *parent)
    : QWidget(parent)
{
	//ui.setupUi(this);
	renderArea = new RenderArea;

	shapeComboBox = new QComboBox;
	shapeComboBox->addItem(tr("Polygon"), Polygon);
	shapeComboBox->addItem(tr("Rectangle"), Rect);
	shapeComboBox->addItem(tr("Rounded Rectangle"), RoundedRect);
	shapeComboBox->addItem(tr("Ellipse"), Ellipse);
	shapeComboBox->addItem(tr("Pie"),  Pie);
	shapeComboBox->addItem(tr("Chord"),  Chord);
	shapeComboBox->addItem(tr("Path"),  Path);
	shapeComboBox->addItem(tr("Line"),  Line);
	shapeComboBox->addItem(tr("Polyline"),  Polyline);
	shapeComboBox->addItem(tr("Arc"),  Arc);
	shapeComboBox->addItem(tr("Points"),  Points);
	shapeComboBox->addItem(tr("Text"),  Text);
	shapeComboBox->addItem(tr("Pixmap"),  Pixmap);

	shapeLabel = new QLabel(tr("&Shape:"));
	shapeLabel->setBuddy(shapeComboBox);

	penWidthSPinBox = new QSpinBox;
	penWidthSPinBox->setRange(0, 20);
	penWidthSPinBox->setSpecialValueText(tr("0 (cosmetic pen)"));

	penWidthLabel = new QLabel(tr("&Pen &Width:"));
	penWidthLabel->setBuddy(penWidthSPinBox);

	penStyleComboBox = new QComboBox;
	penStyleComboBox->addItem(tr("Solid"), Qt::SolidLine);
	penStyleComboBox->addItem(tr("Dash"), Qt::DashLine);
	penStyleComboBox->addItem(tr("Dot"), Qt::DotLine);
	penStyleComboBox->addItem(tr("Dash Dot"), Qt::DashDotLine);
	penStyleComboBox->addItem(tr("Dash Dot Dot"), Qt::DashDotDotLine);
	penStyleComboBox->addItem(tr("None"), Qt::NoPen);

	penStyleLabel = new QLabel(tr("&Pen Style:"));
	penStyleLabel->setBuddy(penStyleComboBox);

	penCapCombobox = new QComboBox;
	penCapCombobox->addItem(tr("Flat"), Qt::FlatCap);
	penCapCombobox->addItem(tr("Square"), Qt::SquareCap);
	penCapCombobox->addItem(tr("Round"), Qt::RoundCap);

	penCapLabel = new QLabel(tr("Pen &Cap:"));
	penCapLabel->setBuddy(penCapCombobox);

	penJoinComboBox = new QComboBox;
	penJoinComboBox->addItem(tr("Miter"), Qt::MiterJoin);
	penJoinComboBox->addItem(tr("Bevel"), Qt::BevelJoin);
	penJoinComboBox->addItem(tr("Round"), Qt::RoundJoin);

	penJoinLabel = new QLabel(tr("Pen &Join:"));
	penJoinLabel->setBuddy(penJoinComboBox);

	brushStyleComboBox = new QComboBox;
	brushStyleComboBox->addItem(tr("Linear Gradient"),
		 Qt::LinearGradientPattern);
	brushStyleComboBox->addItem(tr("Radial Gradient"),
		 Qt::RadialGradientPattern);
	brushStyleComboBox->addItem(tr("Conical Gradient"),
		 Qt::ConicalGradientPattern);
	brushStyleComboBox->addItem(tr("Texture"), Qt::TexturePattern);
	brushStyleComboBox->addItem(tr("Solid"), Qt::SolidPattern);
	brushStyleComboBox->addItem(tr("Horizontal"), Qt::HorPattern);
	brushStyleComboBox->addItem(tr("Vertical"), Qt::VerPattern);
	brushStyleComboBox->addItem(tr("Cross"), Qt::CrossPattern);
	brushStyleComboBox->addItem(tr("Backward Diagonal"), Qt::BDiagPattern);
	brushStyleComboBox->addItem(tr("Forward Diagonal"), Qt::FDiagPattern);
	brushStyleComboBox->addItem(tr("Diagonal Cross"), Qt::DiagCrossPattern);
	brushStyleComboBox->addItem(tr("Dense 1"), Qt::Dense1Pattern);
	brushStyleComboBox->addItem(tr("Dense 2"), Qt::Dense2Pattern);
	brushStyleComboBox->addItem(tr("Dense 3"), Qt::Dense3Pattern);
	brushStyleComboBox->addItem(tr("Dense 4"), Qt::Dense4Pattern);
	brushStyleComboBox->addItem(tr("Dense 5"), Qt::Dense5Pattern);
	brushStyleComboBox->addItem(tr("Dense 6"), Qt::Dense6Pattern);
	brushStyleComboBox->addItem(tr("Dense 7"), Qt::Dense7Pattern);
	brushStyleComboBox->addItem(tr("None"), Qt::NoBrush);

	brushStyleLabel = new QLabel(tr("&Brush Style:"));
	brushStyleLabel->setBuddy(brushStyleComboBox);

	otherOptionsLabel = new QLabel(tr("Other Options:"));
	antialiasingCheckBox = new QCheckBox(tr("&Antialiasing"));
	transformationCheckBox = new QCheckBox(tr("&Transformations"));

	//signal and slot
	connect(shapeComboBox, SIGNAL(activated(int)), this, SLOT(shapeChanged()));
	connect(penWidthSPinBox, SIGNAL(valueChanged(int)), this, SLOT(penChanged()));
	connect(penStyleComboBox, SIGNAL(activated(int)), this, SLOT(penChanged()));
	connect(penCapCombobox, SIGNAL(activated(int)), this, SLOT(penChanged()));
	connect(penJoinComboBox, SIGNAL(activated(int)), this, SLOT(penChanged()));
	connect(brushStyleComboBox, SIGNAL(activated(int)), this, SLOT(brushChanged()));
	connect(antialiasingCheckBox, SIGNAL(toggled(bool)), renderArea, SLOT(setAntialiased(bool)));
	connect(transformationCheckBox, SIGNAL(toggled(bool)),renderArea, SLOT(setTransformed(bool)));

	//Layout
	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->setColumnStretch(0, 1);
	mainLayout->setColumnStretch(3, 1);
	mainLayout->addWidget(renderArea, 0, 0, 1, 4);
	mainLayout->setRowMinimumHeight(1, 6);
	mainLayout->addWidget(shapeLabel, 2, 1, Qt::AlignRight);
	mainLayout->addWidget(shapeComboBox, 2, 2);
	mainLayout->addWidget(penWidthLabel, 3, 1, Qt::AlignRight);
	mainLayout->addWidget(penWidthSPinBox, 3, 2);
	mainLayout->addWidget(penStyleLabel, 4, 1, Qt::AlignRight);
	mainLayout->addWidget(penStyleComboBox, 4, 2);
	mainLayout->addWidget(penCapLabel, 5, 1, Qt::AlignRight);
	mainLayout->addWidget(penCapCombobox, 5, 2);
	mainLayout->addWidget(penJoinLabel, 6, 1, Qt::AlignRight);
	mainLayout->addWidget(penJoinComboBox, 6, 2);
	mainLayout->addWidget(brushStyleLabel, 7, 1, Qt::AlignRight);
	mainLayout->addWidget(brushStyleComboBox, 7, 2);
	mainLayout->setRowMinimumHeight(8, 6);
	mainLayout->addWidget(otherOptionsLabel, 9, 1, Qt::AlignRight);
	mainLayout->addWidget(antialiasingCheckBox, 9, 2);
	mainLayout->addWidget(transformationCheckBox, 10, 2);
	setLayout(mainLayout);

	shapeChanged();
	penChanged();
	brushChanged();
	antialiasingCheckBox->setChecked(true);

	setWindowTitle(tr("Basic Drawing"));

}

WINDOW::~WINDOW()
{

}

void WINDOW::shapeChanged()
{
     Shape shape =  Shape(shapeComboBox->itemData(shapeComboBox->currentIndex(), IdRole).toInt());
    //renderArea->setShape(shape);
    renderArea->setShape(shape);
}

void WINDOW::penChanged()
{
    int width = penWidthSPinBox->value();
    Qt::PenStyle style = Qt::PenStyle(penStyleComboBox->itemData(
            penStyleComboBox->currentIndex(), IdRole).toInt());
    Qt::PenCapStyle cap = Qt::PenCapStyle(penCapCombobox->itemData(
            penCapCombobox->currentIndex(), IdRole).toInt());
    Qt::PenJoinStyle join = Qt::PenJoinStyle(penJoinComboBox->itemData(
            penJoinComboBox->currentIndex(), IdRole).toInt());

    renderArea->setPen(QPen(Qt::blue, width, style, cap, join));
}

void WINDOW::brushChanged()
{
    Qt::BrushStyle style = Qt::BrushStyle(brushStyleComboBox->itemData(
            brushStyleComboBox->currentIndex(), IdRole).toInt());

    if (style == Qt::LinearGradientPattern) {
        QLinearGradient linearGradient(0, 0, 100, 100);
        linearGradient.setColorAt(0.0, Qt::white);
        linearGradient.setColorAt(0.2, Qt::green);
        linearGradient.setColorAt(1.0, Qt::black);
        renderArea->setBrush(linearGradient);
    } else if (style == Qt::RadialGradientPattern) {
        QRadialGradient radialGradient(50, 50, 50, 70, 70);
        radialGradient.setColorAt(0.0, Qt::white);
        radialGradient.setColorAt(0.2, Qt::green);
        radialGradient.setColorAt(1.0, Qt::black);
        renderArea->setBrush(radialGradient);
    } else if (style == Qt::ConicalGradientPattern) {
        QConicalGradient conicalGradient(50, 50, 150);
        conicalGradient.setColorAt(0.0, Qt::white);
        conicalGradient.setColorAt(0.2, Qt::green);
        conicalGradient.setColorAt(1.0, Qt::black);
        renderArea->setBrush(conicalGradient);
    } else if (style == Qt::TexturePattern) {
        renderArea->setBrush(QBrush(QPixmap(":/images/brick.png")));
    } else {
        renderArea->setBrush(QBrush(Qt::green, style));
    }
}
