'''
Довженко А.А. М8О-307Б
Лабораторная работа №1.
Задание: Написать и отладить программу, строящую изображение заданной замечательной кривой.
Вариант №7:
x = a * cos(t)
y = b * sin(t)
'''
from bokeh.io import curdoc
from bokeh.layouts import row, column, widgetbox
from bokeh.models import ColumnDataSource
from bokeh.models.widgets import TextInput
from bokeh.plotting import Figure
import numpy as np
from math import pi

def update_data(attr, old, new):
    try:
        x = float(a.value) * np.cos(t)
        y = float(b.value) * np.sin(t)
        source.data = dict(x=x, y=y)
    except ValueError:
        pass

t = np.linspace(0, 2*pi, 100)
x = np.cos(t)
y = np.sin(t)

source = ColumnDataSource(data=dict(x=x, y=y))

plot = Figure(plot_width=800, x_range=(-100, 100), y_range=(-100, 100), tools="crosshair,pan,reset,save,wheel_zoom")
plot.xaxis.axis_label="x"
plot.yaxis.axis_label="y"
plot.line(x="x", y="y", source=source, line_width=2, legend="x = acos(t)\ny = bsint(t)")
plot.title.text = "Plotting 2D-curves"

a = TextInput(title="a", value='1.0')
b = TextInput(title="b", value='1.0')

for w in [a, b]:
    w.on_change('value', update_data)

inputs = widgetbox(a, b)
curdoc().add_root(row(inputs, plot, width=800))
curdoc().title = "CG lab1 Dovzhenko A. 8O-307"