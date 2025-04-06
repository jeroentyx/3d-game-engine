
#include "VertexArray.h"
#include "../Platform/OpenGLVertexArray.h"

std::shared_ptr<VertexArray> VertexArray::Create()
{
	return std::make_shared<OpenGLVertexArray>();
}