CS101 Project

Shreyas Grampurohit
21D070029

The following REQUIRED FEATURES have been implemented:
(1) The bubble movement has been modified to incorporate vertical motion such that the bubbles travel in a parabolic path. When the ball hits the horizontal ground, it bounces off the ground.
(2) Collision between the bubble and the bullet has been introduced; the bubble disappears after a bullet hits it. After a collision, the bullet also disappears.
(3) Collision between bubble and shooter has been introduced; The effect of this collision is discussed in the 'ADDITIONAL FEATURES'.

Apart from the required features, 2 ADDITIONAL FEATURES have been implemented:
(1) Bubbles of 2 different sizes have been used. When a bullet hits the larger bubbles, they split into smaller bubbles (half the radius of the original larger bubble) and move in horizontally opposite directions.
(2) Score, time and health counters have been added. The score increases whenever a bubble collides with the shooter. The game gets over when we run out of time (measured in seconds) or if we run out of health (whenever the bubble hits the shooter, the health reduces by 1).
Effect of bubble and shooter collision:
a) Health reduces by 1.
b) The original bubble gets destroyed and a new bubble of same radius spawns at a default location with default velocity.

Link to the Google Dive Folder containing the videos:
https://drive.google.com/drive/folders/11qSai2W2rsW6pAS9MGTajAQl6Ht62kpu?usp=sharing