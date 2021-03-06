#include <signet-resolver/cache.h>
#include <signet-resolver/dmtp.h>
#include <signet-resolver/dns.h>
#include <signet-resolver/mrec.h>
#include <signet-resolver/signet-ssl.h>
#include <common/network.h>
#include <common/misc.h>
#include <common/error.h>

int main(void) {

	dime_record_t xdrec;
	dmtp_session_t xsession;
	SSL *s;
	int res;

//	const char *hostname = "technodrome";
//	unsigned short port = 31301;
	const char *hostname = "www.google.com";
//	const char *hostname = "tv.eurosport.com";
//	const char *hostname = "testssl-expire.disig.sk";
	unsigned short port = 443;

	set_dbg_level(3);

	if (!(s = ssl_connect_host(hostname, port, AF_INET))) {
		fprintf(stderr, "Could not connect to host %s:%u\n", hostname, port);
		dump_error_stack();
		exit(EXIT_FAILURE);
	}
//	s = ssl_connect_host("www.wikipedia.org", 443, AF_INET);
//	s = ssl_connect_host("www.google.com", 443, AF_INET);
//	s = ssl_connect_host("www.yahoo.com", 443, AF_INET);
//	s = ssl_connect_host("localhost", 31301, AF_INET);
	memset(&xdrec, 0, sizeof(xdrec));
	memset(&xsession, 0, sizeof(xsession));
	xsession.drec = &xdrec;
	xsession.con = s;
	xsession.domain = (char *)hostname;
	xsession.dx = (char *)hostname;
	load_cache_contents();
	res = verify_dx_certificate(&xsession);
	printf("verification = %d\n", res);

	if (res < 0) {
		fprintf(stderr, "DX certificate verification failure:\n");
		dump_error_stack();
		exit(EXIT_FAILURE);
	}

/*_dump_cache(cached_data_ocsp, 1);
save_cache_contents(); */

	return 0;
}
