#include "ParticleComponent.h"

#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Itexture.h"
#include <iostream>
#include "particleutils.h"
#include "Irandom.h"

#include "Src\ImGui\Context.h"

using std::endl;
using std::cerr;
using glm::vec3;
using glm::mat4;

void Particle_Component::SetMatrices(GLSLProgram& p)
{
	glm::mat4 mv = view * model;
	p.setUniform("MV", mv);
	p.setUniform("Proj", projection);
}

void Particle_Component::compileAndLinkShader()
{
	try
	{
		prog.compileShader("fire_vert.glsl");
		prog.compileShader("fire_frag.glsl");


		GLuint progHandle = prog.getHandle();
		const char* outputNames[] = { "Position", "Velocity", "Age" };
		glTransformFeedbackVaryings(progHandle, 3, outputNames, GL_SEPARATE_ATTRIBS);
		prog.link();


		prog.use();

		flatProg.compileShader("flat_frag.glsl");
		flatProg.compileShader("flat_vert.glsl");
		flatProg.link();
	}
	catch (GLSLProgramException& e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void Particle_Component::initBuffers()
{
	// Generate the buffers
	glGenBuffers(2, posBuf);    // position buffers
	glGenBuffers(2, velBuf);    // velocity buffers
	glGenBuffers(2, age);       // age buffers

	// Allocate space for all buffers
	int size = nParticles * sizeof(GLfloat);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, age[0]);
	glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, age[1]);
	glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_COPY);

	// Fill the first age buffer
	std::vector<GLfloat> initialAge(nParticles);
	float rate = particleLifeTime / nParticles;
	for (int i = 0; i < nParticles; i++) {
		initialAge[i] = rate * (i - nParticles);
	}
	// Shuffle them for better looking results
	Random::shuffle(initialAge);
	glBindBuffer(GL_ARRAY_BUFFER, age[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, initialAge.data());

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Create vertex arrays for each set of buffers
	glGenVertexArrays(2, particleArray);

	// Set up particle array 0
	glBindVertexArray(particleArray[0]);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, age[0]);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);

	// Set up particle array 1
	glBindVertexArray(particleArray[1]);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, age[1]);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	// Setup the feedback objects
	glGenTransformFeedbacks(2, feedback);

	// Transform feedback 0
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, age[0]);

	// Transform feedback 1
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, age[1]);

	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
}

Particle_Component::Particle_Component() : drawBuf(1), time(0), deltaT(0), particleSize(0.5f), particleLifeTime(3.0f), nParticles(70)
{

}


Particle_Component::~Particle_Component()
{

}


void Particle_Component::Init()
{

	setDimension(Eos::Context::GetInstance().scr_width, Eos::Context::GetInstance().scr_height);
	initScene();
	resize(Eos::Context::GetInstance().scr_width, Eos::Context::GetInstance().scr_height);

}

void Particle_Component::initScene()
{
	compileAndLinkShader();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);

	angle = glm::half_pi<float>();
	model = mat4(1.0f);

	initBuffers();

	glActiveTexture(GL_TEXTURE0);
	textureID = Texture::loadTexture(filePath);
	PE_CORE_WARN(filePath);

	glActiveTexture(GL_TEXTURE1);
	ParticleUtils::createRandomTex1D(nParticles * 2);

	prog.use();
	prog.setUniform("RandomTex", 1);
	prog.setUniform("ParticleTex", 0);
	prog.setUniform("ParticleLifetime", particleLifeTime);
	prog.setUniform("ParticleSize", particleSize);
	prog.setUniform("Accel", vec3(0.0f, 0.1f, 0.0f));
	prog.setUniform("Emitter", vec3(0.0f));
	prog.setUniform("Scale", 0.25f);
	prog.setUniform("EmitterBasis", ParticleUtils::makeArbitraryBasis(glm::vec3(0, 1, 0)));

	flatProg.use();
	flatProg.setUniform("Color", glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
	glUseProgram(0);
}

void Particle_Component::update(float t)
{
	deltaT = t - time;
	time = t;
	angle = std::fmod(angle + 0.01f, glm::two_pi<float>());
}

void Particle_Component::PassInUniforms(glm::mat4 _view, glm::mat4 _proj, glm::mat4 _model)
{
	//Updating
	model = _model;
	view = _view;
	projection = _proj;
}

void Particle_Component::render()
{
	// Update pass
	prog.use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	prog.setUniform("Pass", 1);
	prog.setUniform("Time", time);
	prog.setUniform("DeltaT", deltaT);
	prog.setUniform("ParticleLifetime", particleLifeTime);
	glEnable(GL_RASTERIZER_DISCARD);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[drawBuf]);
	glBeginTransformFeedback(GL_POINTS);
	glBindVertexArray(particleArray[1 - drawBuf]);
	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 0);
	glVertexAttribDivisor(2, 0);
	glDrawArrays(GL_POINTS, 0, nParticles);
	glEndTransformFeedback();
	glDisable(GL_RASTERIZER_DISCARD);

	// Render pass
	prog.setUniform("Pass", 2);
	//view = glm::lookAt(vec3(4.0f * cos(angle), 1.5f, 4.0f * sin(angle)), vec3(0.0f, 1.5f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

#ifdef DEBUG
	flatProg.use(); //only drawing grid in debug mode
	SetMatrices(flatProg);
	grid.render();
#endif // DEBUG

	prog.use();
	SetMatrices(prog);
	glDepthMask(GL_FALSE);
	glBindVertexArray(particleArray[drawBuf]);
	glVertexAttribDivisor(0, 1);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nParticles);
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);
	glActiveTexture(GL_TEXTURE0);

	// Swap buffers
	drawBuf = 1 - drawBuf;
	glUseProgram(0);
}

void Particle_Component::resize(int, int)
{
	glViewport(0, 0, Eos::Context::GetInstance().scr_width, Eos::Context::GetInstance().scr_height);
	width = Eos::Context::GetInstance().scr_width;
	height = Eos::Context::GetInstance().scr_height;
	projection = glm::perspective(glm::radians(60.0f), (float)width / height, 0.3f, 100.0f);
}

void Particle_Component::setDimension(int w, int h)
{
	width = Eos::Context::GetInstance().scr_width;
	height = Eos::Context::GetInstance().scr_height;
}

void Particle_Component::SetFilePath(std::string& path)
{
	//Check if path and filePath is not the same
	if (path.compare(filePath))
	{
		filePath = path; //update the current particleComponent filepath
		textureID = Texture::loadTexture(path.c_str());
	}
}
