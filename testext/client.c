#include "hcloud/hcloud.h"

int	main(void)
{
	t_client	client;
	configure_client(&client, "10.11.9.9");

	const char *request = "GET m.html HTTP/1.1\r\nHost: 127.0.0.1\r\n\r\n";
	http_send(&client, request);

	return (0);
}
