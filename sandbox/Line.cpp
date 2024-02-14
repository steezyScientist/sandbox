//#include "Line.hpp"
//
//Line::Line(glm::vec3 origin, glm::vec3 endpoint) {
//	lineOrigin = origin;
//	lineEnd = endpoint;
//	lineColor = glm::vec3(1.0f);
//	ModelViewProjection = glm::mat4(1.0f);
//
//	shader.loadShader("line.vertex", "line.frag");
//
//	float vertices[] = {
//		origin.x, origin.y, origin.z,
//		endpoint.x, endpoint.y, endpoint.z,
//	};
//	
//	glGenVertexArrays(1, &VAO);
//	glGenBuffers(1, &VBO);
//	glBindVertexArray(VAO);
//
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);
//}
//	
//void Line::Draw() {
//	shader.use();
//
//	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)300.0 / (float)300.0, 0.1f, 10.0f);
//	shader.setMat4("projection", projection);
//	glm::mat4 view = camera.GetViewMatrix();
//	shader.setMat4("view", view);
//	glm::mat4 model = glm::mat4(1.0f);
//	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
//	shader.setMat4("model", model);
//
//	glBindVertexArray(VAO);
//	glDrawArrays(GL_LINES, 0, 2);
//}
//
//Line::~Line() {
//	glDeleteVertexArrays(1, &VAO);
//	glDeleteBuffers(1, &VBO);
//	glDeleteProgram(0);
//}