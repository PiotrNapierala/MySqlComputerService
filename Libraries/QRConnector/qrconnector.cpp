/*
This file is part of MySqlComputerService.

Copyright (c) 2023 Piotr Napierała
MySqlComputerService is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see http://www.gnu.org/licenses/.

This program is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License
for the specific language governing permissions and limitations
under the License.
*/

#include "qrconnector.h"

QPixmap QRConnector::GenerateQR(QString text)
{
    QrCode qr = QrCode::encodeText(text.toUtf8(), QrCode::Ecc::MEDIUM);
    QString svg = ToSvgString(qr, 2);
    QSvgRenderer svgRender;
    svgRender.load(svg.toUtf8());
    QPixmap pix(svgRender.defaultSize());
    QPainter pixPainter(&pix);
    svgRender.render(&pixPainter);
    return pix;
}

QString QRConnector::ToSvgString(const QrCode &qr, int border)
{
    if (border < 0)
        throw std::domain_error("Border must be non-negative");
    if (border > INT_MAX / 2 || border * 2 > INT_MAX - qr.getSize())
        throw std::overflow_error("Border too large");

    QString output;
    output += "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    output += "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n";
    output += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" viewBox=\"0 0 ";
    output += QString::number(qr.getSize() + border * 2) + " " + QString::number(qr.getSize() + border * 2) + "\" stroke=\"none\">\n";
    output += "\t<rect width=\"100%\" height=\"100%\" fill=\"#FFFFFF\"/>\n";
    output += "\t<path d=\"";
    for (int y = 0; y < qr.getSize(); y++) {
        for (int x = 0; x < qr.getSize(); x++) {
            if (qr.getModule(x, y)) {
                if (x != 0 || y != 0)
                    output += " ";
                output += "M" + QString::number(x + border) + "," + QString::number(y + border) + "h1v1h-1z";
            }
        }
    }
    output += "\" fill=\"#000000\"/>\n";
    output += "</svg>\n";
    return output;
}
