# CSS Rain Effect

A Pen created on CodePen.io. Original URL: [https://codepen.io/arickle/pen/XKjMZY](https://codepen.io/arickle/pen/XKjMZY).

Looks best with a taller window/layout.

Rain drop elements animate down the screen, stem element inside animates to disappear toward the end of the drop's fall, splat element animates after the stem disappears.

JavaScript used to randomly position drop elements across the screen, slightly randomize their animation duration and delay, and also stagger their top position. Randomizing delay makes sure the drops don't fall in one sheet, randomizing duration keeps them from falling in the same constant synchronization. Staggering top position makes it so the splats don't fall in a single line.

Splatters are unfortunately disabled by default as it is rather intensive and significantly lowers framerate on most machines (in Chrome that is--FireFox and Edge are smoother actually). You can hit the splat toggle to turn it on. The effect is achieved by putting a dotted border-top on an ellipse element, and animating it from scale(0) to scale(1). You can get a better look at it by enabling the single and splat toggles.

There's also a back row you can enable, which is positioned upwards and mirrored.

Does not work well with IE11 at all for whatever reason. Rain drops will not even show unless you jiggle the window size a bit, and at that point all the animations are out of sync.