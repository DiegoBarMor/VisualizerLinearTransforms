import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider, Button

################################################################################
class Grid:
    def __init__(self, size):
        self.size = size
        n = size + 1
        gen_quadrant = lambda a,b: np.array([[[x-a, y-b] for x in range(1,n)] for y in range(1,n)])
        self.quadrants = [
            gen_quadrant(0,0), # TR
            gen_quadrant(n,0), # TL
            gen_quadrant(n,n), # BL
            gen_quadrant(0,n), # BR
        ]

    def get_quadrant(self, q):
        quad = self.quadrants[q]
        x = quad[:,:,0].flat
        y = quad[:,:,1].flat
        return x, y

################################################################################
class Vector:
    def __init__(self, x, y):
        self.x = [0, x]
        self.y = [0, y]
        self.orig_x = 0, x
        self.orig_y = 0, y

    def update(self, ix, iy, jx, jy):
        self.x[1] = self.orig_x[1] * ix + self.orig_y[1] * jx
        self.y[1] = self.orig_x[1] * iy + self.orig_y[1] * jy

################################################################################
class PlotterLinearTransformations:
    def __init__(self, grid_size):
        self.grid = Grid(grid_size)
        self.vectors = [Vector(1,0), Vector(0,1)]
        self.plots = []

    def add_vector(self, x, y):
        self.vectors.append(Vector(x,y))

    def draw_plots(self):
        origin = [-self.grid.size -.5, self.grid.size +.5]
        plt.subplots()
        plt.subplots_adjust(bottom = 0.2, wspace = 0.6)
        plt.axis("square")
        plt.axis(origin * 2)

        plt.plot(origin, [0,0], alpha = .3, color = "black") # x axis
        plt.plot([0,0], origin, alpha = .3, color = "black") # y axis

        for i in range(4): plt.plot(*self.grid.get_quadrant(i), linestyle = "", marker = "+")

        for i,vec in enumerate(self.vectors):
            kwargs = {"marker" : "."}
            if not i:    kwargs["color"] = "blue"
            elif i == 1: kwargs["color"] = "red"
            vector_line, = plt.plot(vec.x, vec.y, **kwargs)
            self.plots.append(vector_line)

        axes = [
            ["ix", (.1, .10, .30, .03)],
            ["iy", (.1, .05, .30, .03)],
            ["jx", (.6, .10, .35, .03)],
            ["jy", (.6, .05, .35, .03)],
        ]
        self.sliders = [
            Slider(
                plt.axes(ax), component, -self.grid.size, self.grid.size, int(component in ["ix", "jy"]), valstep = .5
            ) for component,ax in axes
        ]

        for slid in self.sliders: slid.on_changed(self.update_plot)

    def update_plot(self, val):
        for i,vec in enumerate(self.vectors):
            vec.update(*(slid.val for slid in self.sliders))
            self.plots[i].set_data(vec.x, vec.y)

################################################################################
if __name__ == '__main__':
    plotter = PlotterLinearTransformations(5)
    plotter.add_vector(1, 1)
    plotter.add_vector(3, 2)
    plotter.draw_plots()
    plt.show()

################################################################################
