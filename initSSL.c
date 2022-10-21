#include "general.h"

void initSSL(const char *cacert, const char *key) {
  SSL_library_init();
  OpenSSL_add_all_algorithms();
  SSL_load_error_strings();
  ERR_load_crypto_strings();

  Assert((ctx = SSL_CTX_new(SSLv23_method())) != NULL, "SSLv23 not support");

  SSL_CTX_set_verify(ctx, SSL_VERIFY_FAIL_IF_NO_PEER_CERT, NULL);

  Assert(SSL_CTX_load_verify_locations(ctx, cacert, NULL), "CA cert load fail");
  Assert(SSL_CTX_use_certificate_chain_file(ctx, cacert) > 0, "my cert load fail");
  SSL_CTX_set_default_passwd_cb_userdata(ctx, (void *)"");
  Assert(SSL_CTX_use_PrivateKey_file(ctx, key, SSL_FILETYPE_PEM) > 0, "my private key load fail");

  Assert(SSL_CTX_check_private_key(ctx), "wrong private key");
}
