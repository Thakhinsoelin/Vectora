#pragma once
#include "Core/Core.h"
#include "Core/Application.h"
#include "Events/Event.h"
#include "Events/KeyEvent.h"
#include "Core/Log.h"
#include "Core/LayerStack.h"
#include "imgui/ImGuiLayer.h"
#include "Core/KeyCodes.h"
#include "Core/Input.h"
#include "Core/Timestep.h"
#include "Debug/Instrumentor.h"

//------------Renderer-------------------
#include "Renderer/OrthographiCamera.h"
#include "Renderer/OrthographicCameraController.h"
#include "Renderer/Buffer.h"
#include "Renderer/GraphicsContext.h"
#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Texture.h"
#include "Renderer/FrameBuffer.h"
//----------------------------------------

#include "Scene/Components.h"
#include "Scene/Scene.hpp"
