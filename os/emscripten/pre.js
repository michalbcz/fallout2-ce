Module.noInitialRun = true;

const dropZone = document.createElement('div');
dropZone.style.position = 'fixed';
dropZone.style.top = '0';
dropZone.style.left = '0';
dropZone.style.width = '100vw';
dropZone.style.height = '100vh';
dropZone.style.backgroundColor = 'rgba(0, 0, 0, 0.8)';
dropZone.style.color = 'white';
dropZone.style.display = 'flex';
dropZone.style.alignItems = 'center';
dropZone.style.justifyContent = 'center';
dropZone.style.zIndex = '9999';
dropZone.style.fontSize = '2em';
dropZone.textContent = 'Drop Fallout 2 Installation Folder Here';
document.body.appendChild(dropZone);

dropZone.addEventListener('dragover', (e) => {
    e.preventDefault();
    dropZone.style.backgroundColor = 'rgba(0, 0, 0, 0.9)';
});

dropZone.addEventListener('dragleave', (e) => {
    e.preventDefault();
    dropZone.style.backgroundColor = 'rgba(0, 0, 0, 0.8)';
});

async function traverseFileTree(item, path) {
    path = path || '';
    if (item.isFile) {
        return new Promise((resolve) => {
            item.file((file) => {
                const reader = new FileReader();
                reader.onload = function(e) {
                    const data = new Uint8Array(e.target.result);
                    const fullPath = '/' + path + file.name;
                    // Need to create directories if they don't exist
                    const parts = fullPath.split('/');
                    let currentPath = '';
                    for (let i = 1; i < parts.length - 1; i++) {
                        currentPath += '/' + parts[i];
                        try {
                            FS.mkdir(currentPath);
                        } catch (e) {
                            // Directory might already exist
                        }
                    }
                    FS.writeFile(fullPath, data);
                    resolve();
                };
                reader.readAsArrayBuffer(file);
            });
        });
    } else if (item.isDirectory) {
        const dirReader = item.createReader();
        return new Promise((resolve) => {
            dirReader.readEntries(async (entries) => {
                for (let i = 0; i < entries.length; i++) {
                    await traverseFileTree(entries[i], path + item.name + '/');
                }
                resolve();
            });
        });
    }
}

dropZone.addEventListener('drop', async (e) => {
    e.preventDefault();
    dropZone.textContent = 'Loading...';
    const items = e.dataTransfer.items;

    for (let i = 0; i < items.length; i++) {
        const item = items[i].webkitGetAsEntry();
        if (item) {
            await traverseFileTree(item, '');
        }
    }

    dropZone.textContent = 'Syncing Save Games...';
    try {
        FS.mkdir('/SAVEGAME');
    } catch (e) {}

    FS.mount(IDBFS, {}, '/SAVEGAME');
    FS.syncfs(true, function (err) {
        dropZone.remove();
        Module.callMain();
    });
});
