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

#include "selectededgepathwidget.h"

#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QTextStream>
#include <QVBoxLayout>
#include "../graph/debruijnnode.h"
#include "../program/globals.h"
#include "../program/memory.h"

SelectedEdgePathWidget::SelectedEdgePathWidget(QWidget * parent,
                                               const Path &path,
                                               bool sequencesAvailable,
                                               const QStringList &missingSequenceNodes) :
    QWidget(parent),
    m_path(path),
    m_sequencesAvailable(sequencesAvailable),
    m_missingSequenceNodes(missingSequenceNodes),
    m_infoLabel(new QLabel(this)),
    m_exportFastaButton(new QPushButton("Export FASTA", this)),
    m_exportGafButton(new QPushButton("Export GAF", this))
{
    QVBoxLayout * layout = new QVBoxLayout(this);

    QLabel * title = new QLabel("Path generated from selected edges", this);
    title->setWordWrap(true);
    layout->addWidget(title);

    QLabel * pathLabel = new QLabel("Path: " + path.getString(true), this);
    pathLabel->setWordWrap(true);
    layout->addWidget(pathLabel);

    QString lengthLabelText = "Length: " + formatIntForDisplay(path.getLength()) + " bp";
    if (path.isCircular())
        lengthLabelText += " (circular)";
    QLabel * lengthLabel = new QLabel(lengthLabelText, this);
    layout->addWidget(lengthLabel);

    layout->addWidget(m_infoLabel);

    QHBoxLayout * buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(m_exportGafButton);
    buttonLayout->addWidget(m_exportFastaButton);
    buttonLayout->addStretch();
    layout->addLayout(buttonLayout);

    QString infoText = "Export the path as a GAF walk through the graph.";
    if (m_sequencesAvailable)
        infoText += " Sequence is available, so you can also export FASTA.";
    else
    {
        infoText += " Sequence is missing for: " + m_missingSequenceNodes.join(", ") + ".";
        infoText += " FASTA export is disabled for this path.";
    }
    m_infoLabel->setWordWrap(true);
    m_infoLabel->setText(infoText);

    m_exportFastaButton->setEnabled(m_sequencesAvailable);
    if (!m_sequencesAvailable)
        m_exportFastaButton->setToolTip("FASTA export is unavailable because some nodes lack sequence.");

    connect(m_exportFastaButton, SIGNAL(clicked()), this, SLOT(exportFasta()));
    connect(m_exportGafButton, SIGNAL(clicked()), this, SLOT(exportGaf()));
}


void SelectedEdgePathWidget::exportFasta()
{
    if (!m_sequencesAvailable)
        return;

    QString defaultFileNameAndPath = g_memory->rememberedPath + "/selected_edges_path.fasta";
    QString fileName = QFileDialog::getSaveFileName(this, "Export FASTA", defaultFileNameAndPath,
                                                    "FASTA (*.fasta *.fa);;All files (*)");
    if (fileName == "")
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Export FASTA", "Could not open file for writing:\n" + fileName);
        return;
    }

    QTextStream out(&file);
    out << m_path.getFasta();
    g_memory->rememberedPath = QFileInfo(fileName).absolutePath();
}


void SelectedEdgePathWidget::exportGaf()
{
    QString defaultFileNameAndPath = g_memory->rememberedPath + "/selected_edges_path.gaf";
    QString fileName = QFileDialog::getSaveFileName(this, "Export GAF", defaultFileNameAndPath,
                                                    "GAF (*.gaf);;All files (*)");
    if (fileName == "")
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Export GAF", "Could not open file for writing:\n" + fileName);
        return;
    }

    QList<DeBruijnNode *> nodes = m_path.getNodes();
    QStringList nodeNames;
    for (int i = 0; i < nodes.size(); ++i)
        nodeNames << nodes[i]->getName();

    QString pathField = nodeNames.join(",");
    int pathLength = m_path.getLength();

    QStringList fields;
    fields << "selected_edges_path";
    fields << QString::number(pathLength);
    fields << "0";
    fields << QString::number(pathLength);
    fields << "+";
    fields << pathField;
    fields << QString::number(pathLength);
    fields << "0";
    fields << QString::number(pathLength);
    fields << QString::number(pathLength);
    fields << QString::number(pathLength);
    fields << "255";

    QTextStream out(&file);
    out << fields.join("\t") + "\n";
    g_memory->rememberedPath = QFileInfo(fileName).absolutePath();
}

