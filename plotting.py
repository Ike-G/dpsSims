import plotly.express as px
import plotly.graph_objects as go
import pandas as pd
import re
from math import sqrt


def converttocsv(path,icg):
    with open(path + '.txt', 'r') as f:
        d = f.readlines()
    d = [[int(j) for j in i.strip(' \n').split(' ')] for i in d]
    if icg:
        d = [[1000-i for i in j] if j[-1] < 500 else j for j in d]
    e = [sum([i[j] for i in d])/len(d) for j in range(len(d[0]))]
    v = [(sum([i[j]**2 for i in d])/len(d))-e[j]**2 for j in range(len(d[0]))]
    o = 'L,M,H\n'+'\n'.join([f"{e[i]-sqrt(v[i])},{e[i]},{e[i]+sqrt(v[i])}" for i in range(len(e))])
    with open(path+'.csv', 'x') as out:
        out.write(o)

def parseCSV(limit, *paths):
    colours = ['red', 'green', 'blue', 'goldenrod', 'magenta']
    fig = go.Figure()
    for i in range(len(paths)):
        df = pd.read_csv(paths[i])
        fig.add_trace(
            go.Scatter(x=df.index, y=df[df.index < limit]['L'], name='<extra></extra>', line=dict(color=colours[i], width=2, dash='dash'))
        )
        fig.add_trace(
            go.Scatter(x=df.index, y=df[df.index < limit]['M'], name=paths[i].strip('.csv'), line=dict(color=colours[i], width=2))
        )
        fig.add_trace(
            go.Scatter(x=df.index, y=df[df.index < limit]['H'], name='<extra></extra>', line=dict(color=colours[i], width=2, dash='dash'))
        )
    fig.show()

# data = parseData('data/icg/0-1000-0-0.txt')
# fig = go.Figure([
#     go.Scatter(
#         x=data['t'],
#         y=data['m'],
#         line=dict(color='rgb(0,100,80)'),
#         mode='lines'
#     ),
#     go.Scatter(
#         x=data['t'] + data['t'][::-1],
#         y=data['u'] + data['l'][::-1],
#         fill='toself',
#         fillcolor='rgba(0,100,80,0.2)',
#         line=dict(color='rgba(255,255,255,0)'),
#         hoverinfo="skip",
#         showlegend=False
#     )
# ])
# fig.show()
# fig3 = px.scatter(d2, x="nh", y="t", color="k", log_x=True)
# fig3.show()
