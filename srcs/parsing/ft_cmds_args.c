/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cmds_args.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eviala <eviala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 12:26:11 by eviala            #+#    #+#             */
/*   Updated: 2024/09/26 13:36:33 by eviala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	cnt_args(t_data *data, t_token *token)
{
	int (count) = 0;
	t_token *(tmp) = token;
	if (tmp->type == CMD || (tmp->type == ARG && tmp->prev != data->token->prev
			&& tmp->prev->type > 5))
		count++;
	tmp = tmp->next;
	while (tmp && tmp->type != PIPE)
	{
		if (tmp->type == CMD || (tmp->type == ARG && tmp->prev
				&& tmp->prev->type > 5))
			count++;
		tmp = tmp->next;
	}
	return (count);
}

static int	add_param(char **cmd_param, int *i, char *str)
{
	cmd_param[*i] = ft_strdup(str);
	if (!cmd_param[*i])
		return (0);
	(*i)++;
	return (1);
}

static void	*free_param(char **cmd, int i)
{
	while (--i != -1)
		free(cmd[i]);
	free(cmd);
	return (NULL);
}

char	**get_param(t_data *data, t_token *token)
{
	int (i) = 0;
	int (args_count) = cnt_args(data, token);
	char **(cmd_param) = malloc(sizeof(char *) * (args_count + 1));
	if (cmd_param == NULL)
		return (NULL);
	t_token *(tmp) = token;
	if (tmp->type != PIPE && (tmp->type == CMD || (tmp->type == ARG
				&& tmp->prev != data->token->prev && tmp->prev->type > 5))
		&& !add_param(cmd_param, &i, tmp->str))
		return (free_param(cmd_param, i));
	tmp = tmp->next;
	while (tmp && tmp->type != PIPE)
	{
		if ((tmp->type == CMD || (tmp->type == ARG && tmp->prev
					&& tmp->prev->type > 5)) && !add_param(cmd_param, &i,
				tmp->str))
			return (free_param(cmd_param, i));
		tmp = tmp->next;
	}
	cmd_param[i] = NULL;
	return (cmd_param);
}
