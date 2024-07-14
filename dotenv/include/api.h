#ifndef DOTENV_INCLUDE_API_H
#define DOTENV_INCLUDE_API_H

#define DOTENV_ALWAYS_OVERWRITE 1
#define DOTENV_DEFAULT_ENV_FILE_NAME ".env"

int dotenv_load(char const *env_file);
int dotenv_load_default();

#endif // !DOTENV_INCLUDE_API_H
