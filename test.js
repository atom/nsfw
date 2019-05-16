const nsfw = require('.');

async function main() {
  let watcher = await nsfw('/Users/as-cii/github/fs-tests', console.log);
  await watcher.start();
  watcher.stop();
  await new Promise(resolve => setTimeout(resolve, 250));
  watcher.start();
  watcher = null;
}

main();

setTimeout(() => {
  console.log('GCing');
  global.gc();
}, 4000);
