#include "dlg_iterator.h"
#include "ui_dlg_iterator.h"

#include <algorithm>
#include <iostream>

#include <numeric_range.h>

dlg_iterator::dlg_iterator(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlg_iterator)
{
    ui->setupUi(this);
}

dlg_iterator::~dlg_iterator()
{
    delete ui;
}

void dlg_iterator::on_pbNumericRangeCopy_clicked()
{
    numeric_range<double> range{1.5, 0.5, 5};
    auto first = range.begin();
    auto last = range.end();
    std::copy(first, last, std::ostream_iterator<double>(std::cout, " "));
    std::cout << "\n";
}

void dlg_iterator::on_pbNumericRangeAccumulate_clicked()
{
    numeric_range<double> range{1.5, 0.5, 5};
    std::cout << "Sum = " << std::accumulate(std::begin(range), std::end(range), 0.0) << "\n";
}

void dlg_iterator::on_pbNumericRangeFor_clicked()
{
    numeric_range<double> range{1.5, 0.5, 5};
    for(auto n : range)
    {
        std::cout << n << " ";
    }
    std::cout << "\n";
}
