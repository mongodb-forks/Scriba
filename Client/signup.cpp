#include <QRegularExpression>
#include <QDebug>
#include <QFileDialog>

#include "signup.h"
#include "ui_signup.h"

Signup::Signup(QWidget *parent,Client* client) :
	QWidget(parent),
	ui(new Ui::Signup),
	client(client)
{
	ui->setupUi(this);
	profile=new QPixmap(":/images/anonymous");
    photo=false;

	ui->profile_image->setPixmap(profile->scaled(IMAGE_WIDTH, IMAGE_HEIGHT,
												 Qt::KeepAspectRatioByExpanding)
								 );
	ui->lineEditConfirmPassword->setDisabled(true);
	valid=false;

	connect(client, &Client::signedUp, this, &Signup::signedUp);
	connect(client, &Client::signupError, this, &Signup::signupFailed);
	// Enable signup but pressing 'enter' on confirm password textfield
	connect(ui->lineEditConfirmPassword, &QLineEdit::returnPressed,
			this, &Signup::on_pushButtonSignup_clicked);
}

Signup::~Signup()
{
	delete ui;
}

void Signup::on_pushButtonClear_clicked()
{
	this->clearLineEdit();
	this->clearLabel();
	QPixmap pix(":/images/anonymous");
    photo=false;
	ui->profile_image->setPixmap(pix.scaled(IMAGE_WIDTH, IMAGE_HEIGHT,
											Qt::KeepAspectRatioByExpanding));
	ui->lineEditConfirmPassword->setDisabled(true);
	valid = false;
}

void Signup::signedUp()
{
	// Enable buttons after receiving server reply
	ui->pushButtonSignup->setEnabled(true);
	ui->pushButtonClear->setEnabled(true);
	ui->pushButtonBackLogin->setEnabled(true);

	ui->lineEditUsername->clear();
	ui->lineEditPassword->clear();
	ui->labelInfoPass->setText("Correclty signed up");

	QPixmap pix(":/images/anonymous"); //cercare .png
	ui->profile_image->setPixmap(pix.scaled(IMAGE_WIDTH, IMAGE_HEIGHT,
											Qt::KeepAspectRatioByExpanding));
    photo=false;
    client->disconnectFromHost();
}

void Signup::signupFailed(const QString &reason){
    client->disconnectFromHost();
	ui->labelInfoPass->setText(reason);

	ui->pushButtonSignup->setEnabled(true);
	ui->pushButtonClear->setEnabled(true);
	ui->pushButtonBackLogin->setEnabled(true);
}

void Signup::on_pushButtonSignup_clicked()
{
	QString username=ui->lineEditUsername->text();
	QString password=ui->lineEditPassword->text();
	QString confirm=ui->lineEditConfirmPassword->text();

	if (checkUsername(username) && valid
			&& checkConfirmation(password,confirm)) {
		// Disable buttons before receiving server reply
		ui->pushButtonSignup->setEnabled(false);
		ui->pushButtonClear->setEnabled(false);
		ui->pushButtonBackLogin->setEnabled(false);
        if (photo==false)
             client->signup(username,password,nullptr);
        else
             client->signup(username,password,profile);
        //client->sendProfileImage(username,profile);
	}
}

void Signup::on_lineEditUsername_editingFinished()
{
	QString username = ui->lineEditUsername->text();
	checkUsername(username);
}


void Signup::on_lineEditPassword_editingFinished()
{
	QString password = ui->lineEditPassword->text();
	checkPassword(password);
}

void Signup::on_lineEditConfirmPassword_editingFinished()
{
	QString password1 = ui->lineEditPassword->text();
	QString password2 = ui->lineEditConfirmPassword->text();

	checkConfirmation(password1,password2);
}

void Signup::on_lineEditUsername_textChanged(const QString&)
{
	ui->labelInfoUser->clear();
}

void Signup::on_lineEditPassword_textChanged(const QString& arg)
{
	ui->labelInfoPass->setText("");
	if (arg.size() > 0) {
		ui->lineEditConfirmPassword->setDisabled(false);
	} else {
		ui->lineEditConfirmPassword->setDisabled(true);
		ui->lineEditConfirmPassword->clear();
	}
}

void Signup::on_lineEditConfirmPassword_textChanged(const QString&)
{
	if(valid == true) {
		ui->labelInfoPass->setText("");
	}
}

bool Signup::checkUsername(const QString &username){
	QRegularExpression re("^[A-Za-z0-9\\-_]+(\\.[A-Za-z0-9\\-_]+)*"
						  "@[A-Za-z0-9\\-]+(\\.[A-Za-z0-9\\-]+)*"
						  "(\\.[a-z]{2,6})$");

	QRegularExpressionMatch match = re.match(username);
	bool hasMatch = match.hasMatch();

	if (username.size() > 0 && !hasMatch) {
		ui->labelInfoUser->setText("Invalid email");
		return false;
	}
	return true;
}

bool Signup::isValidPassword(const QString &password, QString& errorMsg,
							 bool& valid) {
	bool success = true;
	// Check password length
	if (password.size() < 8 || password.size() > 12) {
		success = false;
		errorMsg = "Min. 8 characters, Max 12 characters";
	} else {
		errorMsg = "";
		// Set characters not allowed
		QRegularExpression regex("[^A-Za-z0-9@\\.\\-_]");
		QRegularExpressionMatchIterator m = regex.globalMatch(password);
		if (m.hasNext()) {
			success = false;
			errorMsg.append("Characters allowed: A-Z, a-z, 0-9, @ . _ -\n");
		}

		// At least an upper case letter
		regex.setPattern("[A-Z]");
		m = regex.globalMatch(password);
		if (!m.hasNext()) {
			success = false;
			errorMsg.append("Use at least 1 upper case letter\n");
		}

		// At least a lower case letter
		regex.setPattern("[a-z]");
		m = regex.globalMatch(password);
		if (!m.hasNext()) {
			success = false;
			errorMsg.append("Use at least 1 lower case letter\n");
		}

		// At least a digit
		regex.setPattern("[0-9]");
		m = regex.globalMatch(password);
		if (!m.hasNext()) {
			success = false;
			errorMsg.append("Use at least 1 digit\n");
		}

		// At least a special character
		regex.setPattern("[@\\.\\-_]");
		m = regex.globalMatch(password);
		if (!m.hasNext()) {
			success = false;
			errorMsg.append("Use at least 1 special character");
		}
	}
	valid = success;
	return success;
}

void Signup::checkPassword(const QString &password){
	if (password.size() > 0) {
		QString msg;
		bool success = isValidPassword(password, msg, valid);
		if (!success) {
			ui->labelInfoPass->setText(msg);
		}
	}
}

bool Signup::checkConfirmation(const QString &pass,const QString &conf){
	if (conf.size() > 0 && valid == true){
		int res = QString::compare(pass, conf, Qt::CaseSensitive);
		if (res != 0) {
			ui->labelInfoPass->setText("Passwords don't match");
			return false;
		}
	}
	return true;
}

void Signup::on_pushButtonBackLogin_clicked()
{
	this->clearLabel();
	this->clearLineEdit();
	QPixmap image(":/images/anonymous");
	ui->profile_image->setPixmap(image.scaled(IMAGE_WIDTH, IMAGE_HEIGHT,
											  Qt::KeepAspectRatioByExpanding));
    photo=false;
	emit changeWidget(LOGIN);
}

void Signup::clearLabel(){
	ui->labelInfoPass->clear();
	ui->labelInfoUser->clear();
}

void Signup::clearLineEdit(){
	ui->lineEditUsername->clear();
	ui->lineEditPassword->clear();
	ui->lineEditConfirmPassword->clear();
}

void Signup::enableAllButtons()
{
	ui->pushButtonSignup->setEnabled(true);
	ui->pushButtonClear->setEnabled(true);
	ui->pushButtonBackLogin->setEnabled(true);
}

void Signup::on_pushButtonUpload_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(
				this,
				tr("Open Image"),
				QDir::homePath(),
				tr("Image Files (*.png *.jpg *.bmp)")
	);

	if (!fileName.isEmpty() && !fileName.isNull()) {
		profile->load(fileName);
		ui->profile_image->setPixmap(profile->scaled(
											IMAGE_WIDTH,
											IMAGE_HEIGHT,
											Qt::KeepAspectRatioByExpanding));
        photo=true;
	}
}

QPixmap* Signup::getProfile() {
	return profile;
}
