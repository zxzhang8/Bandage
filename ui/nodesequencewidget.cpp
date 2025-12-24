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

#include "nodesequencewidget.h"

#include <QFontDatabase>
#include <QLabel>
#include <QPlainTextEdit>
#include <QVBoxLayout>
#include "../graph/debruijnnode.h"

NodeSequenceWidget::NodeSequenceWidget(QWidget * parent, DeBruijnNode * node) :
    QWidget(parent),
    m_infoLabel(new QLabel(this)),
    m_sequenceEdit(new QPlainTextEdit(this))
{
    QVBoxLayout * layout = new QVBoxLayout(this);

    QLabel * title = new QLabel("Selected node sequence", this);
    title->setWordWrap(true);
    layout->addWidget(title);

    m_infoLabel->setWordWrap(true);
    layout->addWidget(m_infoLabel);

    m_sequenceEdit->setReadOnly(true);
    m_sequenceEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
    m_sequenceEdit->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    layout->addWidget(m_sequenceEdit);

    populateSequence(node);
}


void NodeSequenceWidget::populateSequence(DeBruijnNode * node)
{
    if (node == 0)
    {
        m_infoLabel->setText("No node selected.");
        m_sequenceEdit->setPlainText("");
        return;
    }

    QByteArray fasta = node->getFasta(true);

    QString infoText = "Node: " + node->getName() + " (" + QString::number(node->getLength()) + " bp)";
    if (node->sequenceIsMissing())
        infoText += " - sequence missing, showing Ns.";
    m_infoLabel->setText(infoText);

    m_sequenceEdit->setPlainText(QString::fromUtf8(fasta));
}
