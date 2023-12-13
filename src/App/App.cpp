#include <cmath>
#include <limits>
#include <unordered_map>
#include <array>
#include <cassert>

#include <SDL2pp/Window.hh>
#include <SDL2pp/Renderer.hh>
#include <SDL2pp/Surface.hh>
#include <SDL2pp/Point.hh>
#include <SDL2pp/SDLTTF.hh>
#include <SDL2pp/Font.hh>
#include <SDL2pp/Exception.hh>

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_timer.h>

#include <iostream>

#include "App.hpp"

#define POW2(x) ((x)*(x))
#define WINDOW_WIDTH 1490
#define WINDOW_HEIGHT 810
#define WHITE SDL2pp::Color(255, 255, 255, SDL_ALPHA_OPAQUE)
#define YELLOW SDL2pp::Color(255, 237, 98, SDL_ALPHA_OPAQUE)
#define BLUE SDL2pp::Color(136, 240, 253, SDL_ALPHA_OPAQUE)
#define GREEN SDL2pp::Color(174, 255, 152, SDL_ALPHA_OPAQUE)

App::App() : 
    m_frameRate(),
    m_sdl(SDL_INIT_VIDEO | SDL_INIT_AUDIO),
    m_sdlImage(),
    m_sdlTtf(),
    m_window("Telethon Sub Goal 2023", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED ,WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN),
    m_renderer(m_window, -1, SDL_RENDERER_ACCELERATED),
    m_touch(),
    m_waltographFont("./assets/waltograph.ttf", 60),
    m_waltographFontSmall("./assets/waltograph.ttf", 50),
    m_background(m_renderer, SDL2pp::Surface("assets/christmas-tree.png")),
    m_logoTelethon(m_renderer, SDL2pp::Surface("assets/telethon.png")),
    m_logoFinish(m_renderer, SDL2pp::Surface("assets/telethon_finish.png")),
    m_currentFocus(),
    m_currentGoal()
{
    update();
}

void App::drawBackground()
{
    m_renderer.Copy(m_background, std::nullopt, SDL2pp::Rect(0, 0, m_background.GetWidth(), m_background.GetHeight()));
}

void App::printTitle()
{
    SDL2pp::Texture title { m_renderer, m_waltographFont.RenderText_Blended("Sub Goal Telethon :", WHITE) };
    m_renderer.Copy(title, std::nullopt, SDL2pp::Rect(150, 50, title.GetWidth(), title.GetHeight()));
    m_renderer.Copy(m_logoTelethon, std::nullopt, SDL2pp::Rect(50, 30, title.GetHeight() + 30, title.GetHeight() + 30));

}

void App::printSubGoal()
{
    constexpr std::array<SDL2pp::Color, 4> color { { YELLOW, BLUE, WHITE, GREEN} };
    for (int i = m_currentFocus; i < m_currentFocus +5 && i < static_cast<int>(std::size(m_goal)); i++)
    {
        SDL2pp::Texture goal { m_renderer, m_waltographFontSmall.RenderUTF8_Blended(std::to_string(m_goal[i].prix) + "€ : " + m_goal[i].str_goal, color[i % std::size(color)]) };
        m_renderer.Copy(goal, std::nullopt, SDL2pp::Rect(150, 150 + (i-m_currentFocus) * (130), goal.GetWidth(), goal.GetHeight()));
        if (i < m_currentGoal)
        {
            m_renderer.Copy(m_logoFinish, std::nullopt, SDL2pp::Rect(70, 150 + (i-m_currentFocus) * (130), 50, 50));
        }
        else if (i == m_currentGoal)
        {
            m_renderer.Copy(m_logoTelethon, std::nullopt, SDL2pp::Rect(70, 150 + (i-m_currentFocus) * (130), 50, 50));
        }
    }
}

void App::update()
{
    if (m_frameRate.checkFrame())
    {
        drawBackground();
        printTitle();
        printSubGoal();
        m_renderer.Present();

    }
}

void App::run()
{
    SDL_Event event;
    while (SDL_WaitEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                return;
            case SDL_KEYDOWN:;
                {
                    SDL_KeyCode touch = static_cast<SDL_KeyCode>(event.key.keysym.sym);
                    if (!m_touch.contains(touch) || !m_touch.at(touch))
                    {
                        switch (event.key.keysym.sym)
                        {
                            case SDLK_DOWN:
                                m_touch.insert_or_assign(SDLK_DOWN, true);
                                if (m_currentFocus < static_cast<int>(std::size(m_goal)) - 5) 
                                {
                                    m_currentFocus++;
                                    update();
                                }
                                break;
                            case SDLK_UP:
                                m_touch.insert_or_assign(SDLK_UP, true);
                                if (m_currentFocus > 0) 
                                {
                                    m_currentFocus--;
                                    update();
                                }
                                break;
                            case SDLK_DELETE:
                                m_touch.insert_or_assign(SDLK_DELETE, true);
                                if (m_currentGoal > 0)
                                {
                                    m_currentGoal--;
                                    if (m_currentFocus > 0 && (m_currentGoal +3 >= m_currentFocus && m_currentFocus +3 >= m_currentGoal ))
                                    {
                                        m_currentFocus--;
                                    }
                                    update();
                                }
                                break;
                            case SDLK_RETURN:
                                m_touch.insert_or_assign(SDLK_RETURN, true);
                                if (m_currentGoal < static_cast<int>(std::size(m_goal)))
                                {
                                    m_currentGoal++;
                                    if (m_currentFocus < static_cast<int>(std::size(m_goal)) - 5 && m_currentGoal >= 3 && m_currentGoal +3 >= m_currentFocus && m_currentFocus +3 >= m_currentGoal )
                                    {
                                        m_currentFocus++;
                                    }
                                    update();
                                }
                                break;
                            default:
                                break;
                        }
                    }
                }            
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.sym)
                {
                    case SDLK_DOWN:
                        m_touch.insert_or_assign(SDLK_DOWN, false);
                        break;
                    case SDLK_UP:
                        m_touch.insert_or_assign(SDLK_UP, false);
                        break;
                    case SDLK_DELETE:
                        m_touch.insert_or_assign(SDLK_DELETE, false);
                        break;
                    case SDLK_RETURN:
                        m_touch.insert_or_assign(SDLK_RETURN, false);
                        break;
                    default:
                        break;
                }           
                break;
            default:
                break;
        }
    }
}

bool App::FrameRate::checkFrame()
{
    const unsigned int currentFrame { SDL_GetTicks() };

    if (currentFrame - m_lastFrame >= (1000 / FRAME_RATE))
    {
        m_lastFrame = currentFrame;
        return true;
    }
    else
    {
        return false;
    }
}