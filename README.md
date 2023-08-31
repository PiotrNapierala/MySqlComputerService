# MySqlComputerService
MySqlComputerService is a complete application for managing your own service. It provides the ability to add multiple users, track repair progress, add photos, and include necessary computer repair information.

## Installation
Install the application from the exe installer. Upon the first run, you will be prompted to provide MySQL database access details. No need to create tables, the application will create them automatically. Ensure the database user has full privileges for database management.

## External libraries
The application utilizes several external libraries for sending email messages to clients and generating QR codes. The libraries are:
[nayuki QR-Code-generator](https://github.com/nayuki/QR-Code-generator/tree/master) and [bluetiger9 SmtpClient-for-Qt](https://github.com/bluetiger9/SmtpClient-for-Qt)
