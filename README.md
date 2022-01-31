# Distributed Problem Solving Simulations

This repository contains all programs associated with the experimental data I display within my EPQ, "Investigating the Conditions for Independent Agents to Accomplish Collective Tasks" (Submission date March 2022).

It simulates the learning of agent populations within two different games, according to various strategic update rules described in my EPQ. The style of interaction selection emulates the one described in [Kittock, 1995], and the agent architectures largely follow specifications according to their name, with some modifications made for the multi-agent paradigm.

The iterated coordination game uses rewards +1 and -1 for homogenous and heterogenous action selection, respectively. The iterated prisoner's dilemma gives (3,-3) for betraying a cooperative agent, (1,1) for mutual cooperation, and (-1,-1) for mutual defection.

Within this project there were attempts to make the effects of rewards shift invariant through the implementation of R-learning [Schwartz, 1993], however these failed due to the difficulty of accurately identifying the average reward level within a dynamic system. This finding indicates that in order to sway agents to successfully converge (as was primarily the case within these experiments), there needs to be some external signal indicating that better rewards are possible. In this case, this was implemented purely by having the default rewards generally set to 0, meaning the positive rewards were artificially considered good, and negative rewards were artificially considered bad.
