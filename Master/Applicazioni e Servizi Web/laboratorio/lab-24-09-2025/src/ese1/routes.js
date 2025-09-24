const requestHandler = (req, res) => {
    if (req.url === '/' && req.method === 'GET') {
        res.writeHead(200, { "Content-Type": "text/html" });
        res.write(`
            <html>
                <body>
                    <form action="/message" method="POST">
                        <input type="text" name="message">
                        <button type="submit">Send</button>
                    </form>
                </body>
            </html>
        `);
        return res.end();
    }
    
    if (req.url === '/message' && req.method === 'POST') {
        let body = '';
        req.on('data', chunk => {
            body += chunk;
        });

        req.on('end', () => {
            const message = body.split('=')[1];
            console.log("Message received:", message);

            res.writeHead(302, { "Location": "/" });
            return res.end();
        });
    }
};

exports.handler = requestHandler;
