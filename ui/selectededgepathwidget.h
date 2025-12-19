//Copyright 2024

//This file is part of Bandage.

//Bandage is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//Bandage is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with Bandage.  If not, see <http://www.gnu.org/licenses/>.

#ifndef SELECTEDEDGEPATHWIDGET_H
#define SELECTEDEDGEPATHWIDGET_H

#include <QWidget>
#include <QStringList>
#include "../graph/path.h"

class QLabel;
class QPushButton;

class SelectedEdgePathWidget : public QWidget
{
    Q_OBJECT

public:
    SelectedEdgePathWidget(QWidget * parent,
                           const Path &path,
                           bool sequencesAvailable,
                           const QStringList &missingSequenceNodes);

private:
    Path m_path;
    bool m_sequencesAvailable;
    QStringList m_missingSequenceNodes;
    QLabel * m_infoLabel;
    QPushButton * m_exportFastaButton;
    QPushButton * m_exportGafButton;

private slots:
    void exportFasta();
    void exportGaf();
};

#endif // SELECTEDEDGEPATHWIDGET_H
