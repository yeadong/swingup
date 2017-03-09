import sys, random
import pygame
from pygame.locals import *
import pymunk
import pymunk.pygame_util
from pymunk import Vec2d
# let's initialize our globals:
FRAMERATE = 60.0

#colors
BLACK = (0, 0, 0,)
WHITE = (255, 255, 255,)

DISPLAY_WIDTH = 1200
DISPLAY_HEIGHT = 675

def convert_to_pygame_coords(p):
    """Small hack to convert pymunk to pygame coordinates"""
    return int(p.x), int(-p.y+600)

def setup_pole(mass=0.4, radius=0, size=(10, 200)):
    moment = pymunk.moment_for_box(mass, size)
    pole_body = pymunk.Body(mass, moment)
    pole_body.position = (DISPLAY_WIDTH/2, DISPLAY_HEIGHT / 2 - 100)
    pole_body.start_position = Vec2d(pole_body.position)
    pole_shape = pymunk.Poly.create_box(pole_body, size, radius)
    pole_shape.elasticity = 1
    return (pole_body, pole_shape)

def setup_agent():
    ### agent body
    agent_body = pymunk.Body(2, pymunk.inf)
    agent_body.position = DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2

    agent_shape = pymunk.Segment(agent_body, (-50,0), (50,0), 30)
    agent_shape.color = (40, 40, 40,)

    return (agent_body, agent_shape)

# the main function is, expectedly, where the magic happens
# this function will be run everytime we start our program
def main():

    running = True

    pygame.init()
    screen = pygame.display.set_mode((DISPLAY_WIDTH, DISPLAY_HEIGHT))
    pygame.display.set_caption('SwingUp!')
    clock = pygame.time.Clock()

    ### init our pymunk space and apply gravity to it
    space = pymunk.Space()
    space.gravity = (0.0, -4000)


    # to prevent the pendulum from blowing up, we introduce
    # a system wide damping
    space.damping = 0.7



    draw_options = pymunk.pygame_util.DrawOptions(screen)


    agent_body, agent_shape = setup_agent()
    pole_body, pole_shape = setup_pole()

    move_x_joint = pymunk.GrooveJoint(space.static_body, agent_body, (0, DISPLAY_HEIGHT / 2), (DISPLAY_WIDTH, DISPLAY_HEIGHT / 2), (0,0))
    pj = pymunk.PinJoint(pole_body, agent_body, (0,100,), (0, 0))
    pj.distance = 0
    pj.collide_bodies = False
    pj.error_bias = 0
    space.add(agent_body, agent_shape, pole_body, pole_shape, move_x_joint, pj)

    keys = {
        'left': False,
        'right': False
    }

    force_direction = 0

    # We want to apply a constant force to our dynamic cart as we are keeping
    # our respective arrow key pressed. If we were to apply this force in our
    # event for loop, this would lead us to a situation in which the force
    # would be applied in one single step only as we are solely tracking
    # keyup / keydown behaviours for performance reasons. Hence, we rather
    # need to keep track on which action should be executed and perform on
    # every step, meaning after our for event loop which figures out whether
    # something changed.
    #
    # To achieve this behaviour, we introduce a variable named `force_direction`
    # which will act as an indicator if we need to apply force and if so,
    # as a multiplier in order to apply the right direction:
    #
    #             y ^
    #               |
    #               |
    #               |
    #  ------------------------- > x
    #  <- negative  |  positive ->
    #  action= - 1  |   action = 1
    #               |
    #
    #           action = 0
    #
    # + If we do not have any user input, action is 0
    # + If the user presses `<` , action is -1
    # + If the user presses `>` , action is 1
    # + If the user releases `<` or `>` a and no other key is pressed
    #   action is 0
    # + If the user releases `<` or `>` and the opposite direction key
    #   meaning `>` or `<` is pressed, the action is inverted:
    #   -1 -> 1 | 1 -> -1

    while running:

        for event in pygame.event.get():
            if event.type == QUIT or \
                event.type == KEYDOWN and (event.key in [K_ESCAPE, K_q]):
                running = False
                break

            elif event.type == KEYDOWN and event.key == K_LEFT:
                keys['left'] = True
                force_direction = -1

            elif event.type == KEYUP and event.key == K_LEFT:
                keys['left'] = False
                if not keys['right']:
                    force_direction = 0
                else:
                    force_direction = 1

            elif event.type == KEYDOWN and event.key == K_RIGHT:
                keys['right'] = True
                force_direction = 1

            elif event.type == KEYUP and event.key == K_RIGHT:
                keys['right'] = False
                if not keys['left']:
                    force_direction = 0
                else:
                    force_direction = -1

        f = (force_direction*4000, 0)
        print(f)
        agent_body.apply_force_at_local_point(f, agent_body.position)
        print(agent_body)

        # NOTE: `step` HAS to be float/double
        space.step(1/FRAMERATE)

        screen.fill(WHITE)
        space.debug_draw(draw_options)

        pygame.display.flip()
        clock.tick(FRAMERATE)

if __name__ == '__main__':
    sys.exit(main())
