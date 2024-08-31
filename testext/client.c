#include "hcloud/hcloud.h"

int	main(void)
{
	t_client	client;
	configure_client(&client, "107.71.1.1");

	const char *request = "GET m.html HTTP/1.1\r\nHost: 107.71.1.1\r\n\r\n";
	http_send(&client, request);

	return (0);
}
