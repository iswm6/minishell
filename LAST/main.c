/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkenned <fkenned@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/07 16:26:26 by fkenned           #+#    #+#             */
/*   Updated: 2021/11/07 17:17:38 by fkenned          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main_process(t_data *data, char ***envp)
{
	if (!data->line)
	{
		write(1, "\n", 1);
		return (1);
	}
	if (data->line[0] != '\0')
	{
		parser(&(*data));
		if (data->error == NULL && data->line[0] != '\0')
			pipex(&(*data), envp);
		else
		{
			unlink_here_doc_files(&(*data));
			free(data->error);
		}
		free(data->line);
		clean_position(data->pos_pipe);
		cleaning_all(data->pipes);
	}
	else
		free(data->line);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	g_question = 0;
	data.get_pid = 0;
	envp = make_new_envp(envp);
	init_data(&data, &envp);
	get_pid(&data, envp);
	signal(SIGQUIT, ctrl_slash);
	signal(SIGINT, ctrl_c);
	while (isatty(0))
	{
		data.line = readline("MINISHELL$ ");
		add_history(data.line);
		if (main_process(&data, &envp) == 1)
			break ;
	}
	free_array(envp);
	clean_exp_env(data.env);
	clean_exp_env(data.exp);
	return (0);
}