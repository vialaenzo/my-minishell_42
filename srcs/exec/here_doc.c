/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eviala <eviala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 12:04:23 by eviala            #+#    #+#             */
/*   Updated: 2024/09/20 09:28:37 by eviala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_sigint_child(int signum)
{
	if (signum == SIGINT)
	{
		exit(130);
	}
}

static bool	read_heredoc(t_data *data, char *limiter)
{
	int		fd;

	char (*buf) = NULL;
	signal(SIGINT, &handle_sigint_child);
	while (1)
	{
		buf = readline("\033[1;93mheredoc>\033[0m ");
		if (!buf)
		{
			ft_error("Buf readline");
			break ;
		}
		if (ft_strncmp(limiter, buf, ft_strlen(limiter) + 1) == 0)
			break ;
		if (!ft_expand(&buf, data))
			free_everything(data, "Allocation failed", 1);
		fd = open(".heredoc.tmp", O_CREAT | O_WRONLY | O_APPEND, 0644);
		if (fd < 0)
			return (ft_error("Failed to create temp file"), false);
		ft_printf(fd, "%s\n", buf);
		close(fd);
		free(buf);
	}
	return (free(buf), close(fd), true);
}

int	here_doc(t_data *data, char *limiter)
{
	int		status;

	pid_t (pid) = fork();
	if (pid < 0)
		return (-1);
	else if (pid == 0)
	{
		if (!read_heredoc(data, limiter))
		{
			unlink(".heredoc.tmp");
			exit(1);
		}
		exit(0);
	}
	else
	{
		waitpid(pid, &status, 0);
		g_signal = WEXITSTATUS(status);
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
			return (unlink(".heredoc.tmp"), -1);
		int (fd) = open(".heredoc.tmp", O_RDONLY);
		if (fd < 0)
			return (unlink(".heredoc.tmp"), -1);
		return (unlink(".heredoc.tmp"), fd);
	}
}
