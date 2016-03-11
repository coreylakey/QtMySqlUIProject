#include "adminreports.h"
#include "ui_adminreports.h"
#include <QSqlQuery>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlRecord>


AdminReports::AdminReports(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdminReports)
{
    ui->setupUi(this);
    //Sets fixed size for entire window
    this->setMaximumSize(1440,900);
    this->setMinimumSize(1440,900);
    this->setWindowTitle("South Coast Family Harbor");   //NOTE: Window Titles is not changing properly.

    //Tiles background image
    QPalette *Palette = new QPalette();
    QPixmap *Pixmap = new QPixmap("/home/corey/Desktop/SCFHCOPY/bg_tile.jpg");
    Palette->setBrush(QPalette::Background,QBrush(*Pixmap));
    setPalette(*Palette);

//Labels
    clientLbl = new QLabel("How many clients", this);
    secClientLbl = new QLabel("", this);
    giftLbl = new QLabel("got", this);
    result = new QLabel("Total: 0", this);
    fromLbl = new QLabel("From:", this);
    toLbl = new QLabel("To:", this);
    dateLbl = new QLabel("Use Dates?", this);
//Labels Positions
    clientLbl->setGeometry(QRect(30,20,300,30));
    secClientLbl->setGeometry(QRect(30,60,300,30));
    giftLbl->setGeometry(QRect(30,100,300,30));
    result->setGeometry(QRect(30,500,300,30));
    fromLbl->setGeometry(QRect(150, 180, 50, 30));
    toLbl->setGeometry(QRect(520, 180, 50, 30));
    dateLbl->setGeometry(QRect(30, 140, 300, 30));
//Label Fonts
    clientLbl->setStyleSheet("QLabel {font : bold;}");
    secClientLbl->setStyleSheet("QLabel {font : bold;}");
    giftLbl->setStyleSheet("QLabel {font : bold;}");
    result->setStyleSheet("QLabel {font : bold 24px;}");
    fromLbl->setStyleSheet("QLabel {font : bold;}");
    toLbl->setStyleSheet("QLabel {font : bold;}");
    dateLbl->setStyleSheet("QLabel {font : bold;}");
//CheckBoxes Creation
    yesBox   = new QCheckBox(this);
    noBox = new QCheckBox(this);
//CheckBoxes Attributes
    yesBox->setGeometry(QRect(120, 140, 50, 30));
    yesBox->setText("Yes");
    noBox->setGeometry(QRect(180, 140, 50, 30));
    noBox->setText("No");
//Make Gender Exclusive
    dateGroup = new QButtonGroup(this);
    dateGroup->addButton(yesBox);
    dateGroup->addButton(noBox);
    dateGroup->setExclusive(true);
//ComboBoxes
    clientEdit = new QComboBox(this);
    secClientEdit = new QComboBox(this);
    giftEdit = new QComboBox(this);
//ComboBox Positions
    clientEdit->setGeometry(QRect(150,20,300,30));
    secClientEdit->setGeometry(QRect(150,60,300,30));
    giftEdit->setGeometry(QRect(150,100,300,30));
//Populate the ComboBoxes
    QStringList clientList;
    QStringList giftList;
//One of these lists will populate secCLientList once they have chosen a criteria in clientList
    clientList      << "Total" << "City" << "Living Situation" << "Housing Type" << "Income Source" << "How Heard" << "Child Age" << "Child Gender";
//Gifts they can query by:
    giftList << "Total" << "Diapers" << "Wipes" << "Blankets" << "Baby Lotion" << "Baby Wash" << "Baby Powder" << "Diaper Cream" << "Toothbrushes" << "Toothpaste" << "Bottles" << "Sippy Cups" << "Plastic Plates"
             << "Clothes" << "Socks" << "Shoes" << "Misc.";
//Put strings into ComboBoxes
    clientEdit->addItems(clientList);
    giftEdit->addItems(giftList);
    // secClientEdit will be populated depending on which criteria is entered in cliendEdit.

//Calendar Creation and Position
    fromCalendar = new QCalendarWidget(this);
    fromCalendar->setGeometry(QRect(200, 180, 300, 250));
    toCalendar = new QCalendarWidget(this);
    toCalendar->setGeometry(QRect(550, 180, 300, 250));

//Buttons
    run = new QPushButton(this);
    cancel = new QPushButton(this);
    csvExport = new QPushButton(this);
//Button Positions
    run->setGeometry(QRect(800,800,150,30));
    cancel->setGeometry(QRect(950,800,150,30));
    csvExport->setGeometry(QRect(1100,800,150,30));
//Button Text
    run->setText("Run Query");
    cancel->setText("Cancel");
    csvExport->setText("Export");

//Prepare for usage.
    secClientLbl->hide();
    giftLbl->hide();
    secClientEdit->hide();
    giftEdit->hide();
    dateLbl->hide();
    fromLbl->hide();
    toLbl->hide();
    fromCalendar->hide();
    toCalendar->hide();
    yesBox->hide();
    noBox->hide();

//CONNECTIONS
    connect(this->clientEdit,SIGNAL( currentIndexChanged(int)),this,SLOT( firstClientChange() ));
    connect(this->run, SIGNAL( clicked()), this, SLOT( runQuery() ));
    connect(this->cancel, SIGNAL( clicked()),this, SLOT(close() ));
    connect(this->dateGroup, SIGNAL(buttonClicked(int)),this, SLOT( setCalendar() ));
    connect(this->csvExport, SIGNAL(clicked()),this, SLOT( exportToCSV() ));
}

void AdminReports::exportToCSV()
{
    // MySql only has access to tmp folder. the query is : SELECT * INTO OUTFILE '/tmp/result.csv' FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"' LINES TERMINATED BY '\n' FROM clients;
    // You can only do csv dumps like this one table at a time.. so each table will need to be dumped and then individually loaded to excel.

    QSqlQuery exQuery;
    exQuery.prepare("SELECT * INTO OUTFILE '/tmp/clientTable.csv' FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '\"' LINES TERMINATED BY '\n' FROM clients;");
    exQuery.exec();
    exQuery.prepare("SELECT * INTO OUTFILE '/tmp/giftsTable.csv' FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '\"' LINES TERMINATED BY '\n' FROM gifts;");
    exQuery.exec();

    return;

}

void AdminReports::runQuery()
{



}

void AdminReports::setCalendar()
{
    if( yesBox->isChecked() )
    {
        fromCalendar->show();
        toCalendar->show();
        fromLbl->show();
        toLbl->show();
    }
    else if( noBox->isChecked() )
    {
        fromCalendar->hide();
        toCalendar->hide();
        fromLbl->hide();
        toLbl->hide();
    }

    return;

}



void AdminReports::firstClientChange()
{
    QStringList cityList;
    QStringList childAgeList;
    QStringList livSitList;
    QStringList houseTypeList;
    QStringList incSrcList;
    QStringList howHeardList;
    QStringList childGenderList;
    cityList        << "CB" << "NB" << "MP" << "CQ" << "BN" << "LKS" << "REED" << "P.O." << "Powers" << "Other";
    childAgeList    << "Prenatal" << "1mo" << "2mo" << "3mo" << "4mo" << "5mo" << "6mo" << "24mo" << "3 years" << "4 years" << "5 years" << "6 years";
    livSitList      << "Friends" << "Family" << "Self";
    houseTypeList   << "HVD" << "Rent" << "Own";
    incSrcList      << "TANF" << "Work" << "SNAP" << "Child Support" << "Foster Care" << "Disability" << "Self-Employed" << "Unemployment" << "Alimony" << "None";
    howHeardList    << "Friend" << "Family" << "Newmark Center" << "HeadStart" << "Maslow" << "Local Ads" << "Facebook" << "Other";
    childGenderList << "Male" << "Female";

    int index = clientEdit->currentIndex();
    switch( index )
    {
        case 0:
            secClientEdit->hide();
            secClientEdit->clear();
            secClientLbl->setText("In total...");
            return;
            break;
        case 1:
            secClientLbl->setText("Are From");
            secClientEdit->clear();
            secClientEdit->addItems(cityList);
            break;
        case 2:
            secClientLbl->setText("Living Situation is");
            secClientEdit->clear();
            secClientEdit->addItems(livSitList);
            break;
        case 3:
            secClientLbl->setText("Housing Situation is");
            secClientEdit->clear();
            secClientEdit->addItems(houseTypeList);
            break;
        case 4:
            secClientLbl->setText("Income Source is");
            secClientEdit->clear();
            secClientEdit->addItems(incSrcList);
            break;
        case 5:
            secClientLbl->setText("Heard About us by");
            secClientEdit->clear();
            secClientEdit->addItems(howHeardList);
            break;
        case 6:
            secClientLbl->setText("Childs Age is");
            secClientEdit->clear();
            secClientEdit->addItems(childAgeList);
            break;
        case 7:
            secClientLbl->setText("Childs Gender is");
            secClientEdit->clear();
            secClientEdit->addItems(childGenderList);
            break;
        default:
            break;
    }

    secClientLbl->show();
    secClientEdit->show();
    giftLbl->show();
    giftEdit->show();
    dateLbl->show();
    yesBox->show();
    noBox->show();
}

AdminReports::~AdminReports()
{
    delete ui;
}
